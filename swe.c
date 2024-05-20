#include <petscts.h>
#include <petscdmda.h>
#include <math.h>
#include <stdio.h>

#define g 9.81  // Gravity constant

typedef struct {
    PetscReal dx;    // Spatial grid spacing
    PetscInt  nx;    // Number of spatial points
} AppCtx;

// Manufactured solution and source terms
static PetscErrorCode ManufacturedSolution(PetscReal x, PetscReal t, PetscScalar *h, PetscScalar *hu) {
    PetscReal h_val = 1.0 + 0.1 * PetscSinReal(PETSC_PI * x) * PetscCosReal(PETSC_PI * t);
    PetscReal u_val = 0.1 * PetscCosReal(PETSC_PI * x) * PetscSinReal(PETSC_PI * t);
    *h = h_val;
    *hu = h_val * u_val;
    return 0;
}

// Source term for h
static PetscErrorCode SourceTermH(PetscReal x, PetscReal t, PetscScalar *S_h) {
    PetscReal d_h_dt = -0.1 * PetscSinReal(PETSC_PI * x) * PetscSinReal(PETSC_PI * t) * PETSC_PI;
    *S_h = d_h_dt;
    return 0;
}

// Source term for hu
static PetscErrorCode SourceTermHu(PetscReal x, PetscReal t, PetscScalar *S_hu) {
    PetscReal h_val = 1.0 + 0.1 * PetscSinReal(PETSC_PI * x) * PetscCosReal(PETSC_PI * t);
    PetscReal u_val = 0.1 * PetscCosReal(PETSC_PI * x) * PetscSinReal(PETSC_PI * t);
    PetscReal d_hu_dt = h_val * 0.1 * PetscCosReal(PETSC_PI * x) * PetscCosReal(PETSC_PI * t) * PETSC_PI +
                        u_val * (-0.1 * PetscSinReal(PETSC_PI * x) * PetscSinReal(PETSC_PI * t) * PETSC_PI);
    *S_hu = d_hu_dt;
    return 0;
}

// Function to write the solution to a file
static PetscErrorCode WriteSolutionToFile(DM da, Vec U, const char *filename) {
    DMDALocalInfo info;
    PetscScalar **u;
    FILE *file;
    PetscInt i;
    PetscReal x, dx;

    PetscFunctionBeginUser;
    PetscCall(DMDAGetLocalInfo(da, &info));
    PetscCall(DMDAVecGetArrayDOFRead(da, U, &u));

    dx = 1.0 / (info.mx - 1);
    file = fopen(filename, "w");
    if (!file) SETERRQ(PETSC_COMM_WORLD, PETSC_ERR_FILE_OPEN, "Cannot open file");

    fprintf(file, "x,h,hu\n");
    for (i = 0; i < info.mx; i++) {
        x = i * dx;
        fprintf(file, "%g,%g,%g\n", (double)x, (double)u[i][0], (double)u[i][1]);
    }

    fclose(file);
    PetscCall(DMDAVecRestoreArrayDOFRead(da, U, &u));
    PetscFunctionReturn(0);
}

static PetscErrorCode RHSFunction(TS ts, PetscReal t, Vec U, Vec F, void *ctx) {
    AppCtx *user = (AppCtx*)ctx;
    DM da;
    DMDALocalInfo info;
    PetscScalar **u, **f;
    Vec Ulocal;
    PetscInt i;
    PetscReal dx = user->dx;
    PetscScalar S_h, S_hu, x;

    PetscFunctionBeginUser;
    PetscCall(TSGetDM(ts, &da));
    PetscCall(DMDAGetLocalInfo(da, &info));

    PetscCall(DMGetLocalVector(da, &Ulocal));
    PetscCall(DMGlobalToLocalBegin(da, U, INSERT_VALUES, Ulocal));
    PetscCall(DMGlobalToLocalEnd(da, U, INSERT_VALUES, Ulocal));

    PetscCall(DMDAVecGetArrayDOFRead(da, Ulocal, &u));
    PetscCall(DMDAVecGetArrayDOF(da, F, &f));

    for (i = info.xs; i < info.xs + info.xm; i++) {
        x = i * dx;
        PetscCall(SourceTermH(x, t, &S_h));
        PetscCall(SourceTermHu(x, t, &S_hu));
        if (i == 0 || i == info.mx - 1) {
            f[i][0] = 0.0;
            f[i][1] = 0.0;
        } else {
            f[i][0] = -((u[i+1][1] - u[i-1][1]) / (2*dx)) + S_h;
            if (u[i+1][0] != 0 && u[i-1][0] != 0) {
                f[i][1] = -((u[i+1][1] * u[i+1][1] / u[i+1][0] + 0.5 * g * u[i+1][0] * u[i+1][0]) - 
                            (u[i-1][1] * u[i-1][1] / u[i-1][0] + 0.5 * g * u[i-1][0] * u[i-1][0])) / (2*dx) + S_hu;
            } else {
                PetscPrintf(PETSC_COMM_WORLD, "Division by zero detected at index %d\n", i);
                f[i][1] = 0.0;
            }
        }
    }

    PetscCall(DMDAVecRestoreArrayDOFRead(da, Ulocal, &u));
    PetscCall(DMDAVecRestoreArrayDOF(da, F, &f));
    PetscCall(DMRestoreLocalVector(da, &Ulocal));
    PetscFunctionReturn(0);
}

static PetscErrorCode InitialCondition(DM da, Vec U) {
    DMDALocalInfo info;
    PetscScalar **u;
    PetscInt i;
    PetscReal x, dx;

    PetscFunctionBeginUser;
    PetscCall(DMDAGetLocalInfo(da, &info));
    PetscCall(DMDAVecGetArrayDOF(da, U, &u));

    dx = 1.0 / (info.mx - 1);
    for (i = info.xs; i < info.xs + info.xm; i++) {
        x = i * dx;
        PetscCall(ManufacturedSolution(x, 0.0, &u[i][0], &u[i][1]));
    }

    PetscCall(DMDAVecRestoreArrayDOF(da, U, &u));
    PetscFunctionReturn(0);
}

static PetscErrorCode ExactSolution(DM da, Vec U_exact, PetscReal t) {
    DMDALocalInfo info;
    PetscScalar **u_exact;
    PetscInt i;
    PetscReal x, dx;

    PetscFunctionBeginUser;
    PetscCall(DMDAGetLocalInfo(da, &info));
    PetscCall(DMDAVecGetArrayDOF(da, U_exact, &u_exact));

    dx = 1.0 / (info.mx - 1);
    for (i = info.xs; i < info.xs + info.xm; i++) {
        x = i * dx;
        PetscCall(ManufacturedSolution(x, t, &u_exact[i][0], &u_exact[i][1]));
    }

    PetscCall(DMDAVecRestoreArrayDOF(da, U_exact, &u_exact));
    PetscFunctionReturn(0);
}

static PetscErrorCode ComputeErrorNorm(DM da, Vec U, Vec U_exact, PetscReal *norm_h, PetscReal *norm_hu) {
    Vec error;
    PetscScalar **u, **u_exact, **err;
    DMDALocalInfo info;
    PetscInt i;
    PetscReal sum_h = 0.0, sum_hu = 0.0, dx;

    PetscFunctionBeginUser;
    PetscCall(DMDAGetLocalInfo(da, &info));
    PetscCall(DMCreateLocalVector(da, &error));
    PetscCall(DMDAVecGetArrayDOFRead(da, U, &u));
    PetscCall(DMDAVecGetArrayDOFRead(da, U_exact, &u_exact));
    PetscCall(DMDAVecGetArrayDOF(da, error, &err));

    dx = 1.0 / (info.mx - 1);
    for (i = info.xs; i < info.xs + info.xm; i++) {
        err[i][0] = u[i][0] - u_exact[i][0];
        err[i][1] = u[i][1] - u_exact[i][1];
        sum_h += err[i][0] * err[i][0];
        sum_hu += err[i][1] * err[i][1];
    }

    PetscCall(DMDAVecRestoreArrayDOFRead(da, U, &u));
    PetscCall(DMDAVecRestoreArrayDOFRead(da, U_exact, &u_exact));
    PetscCall(DMDAVecRestoreArrayDOF(da, error, &err));

    MPI_Allreduce(&sum_h, norm_h, 1, MPIU_REAL, MPI_SUM, PETSC_COMM_WORLD);
    MPI_Allreduce(&sum_hu, norm_hu, 1, MPIU_REAL, MPI_SUM, PETSC_COMM_WORLD);

    if (sum_h != 0) {
        *norm_h = sqrt(*norm_h * dx);
    } else {
        *norm_h = 0.0;
    }

    if (sum_hu != 0) {
        *norm_hu = sqrt(*norm_hu * dx);
    } else {
        *norm_hu = 0.0;
    }

    PetscCall(VecDestroy(&error));
    PetscFunctionReturn(0);
}

int main(int argc, char **argv) {
    TS ts;
    Vec U, U_exact;
    DM da;
    AppCtx user;
    PetscReal norm_h, norm_hu, t_final = 1.0;
    PetscInt grids[] = {1000};  // grid resolution
    PetscInt i;

    PetscFunctionBeginUser;
    PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));

    for (i = 0; i < sizeof(grids) / sizeof(grids[0]); i++) {
        user.nx = grids[i];
        user.dx = 1.0 / (user.nx - 1);

        PetscCall(DMDACreate1d(PETSC_COMM_WORLD, DM_BOUNDARY_NONE, user.nx, 2, 1, NULL, &da));
        PetscCall(DMSetFromOptions(da));
        PetscCall(DMSetUp(da));

        PetscCall(DMCreateGlobalVector(da, &U));
        PetscCall(DMCreateGlobalVector(da, &U_exact));

        PetscCall(InitialCondition(da, U));
        PetscCall(ExactSolution(da, U_exact, 0.0));  // exact solution at initial time

        PetscCall(TSCreate(PETSC_COMM_WORLD, &ts));
        PetscCall(TSSetDM(ts, da));
        PetscCall(TSSetProblemType(ts, TS_NONLINEAR));
        PetscCall(TSSetRHSFunction(ts, NULL, RHSFunction, &user));
        PetscCall(TSSetType(ts, TSEULER));
        PetscCall(TSSetTimeStep(ts, 0.001));  // Reduced time step
        PetscCall(TSSetMaxTime(ts, t_final));
        PetscCall(TSSetExactFinalTime(ts, TS_EXACTFINALTIME_MATCHSTEP));

        PetscCall(TSSetFromOptions(ts));
        PetscCall(TSSolve(ts, U));

        PetscCall(ExactSolution(da, U_exact, t_final));  // Updates exact solution to final time
        PetscCall(ComputeErrorNorm(da, U, U_exact, &norm_h, &norm_hu));
        PetscPrintf(PETSC_COMM_WORLD, "Grid size: %d, Error norm (h): %g, Error norm (hu): %g\n", user.nx, (double)norm_h, (double)norm_hu);

        // Write solution to file
        char filename[256];
        sprintf(filename, "solution_%d.csv", user.nx);
        PetscCall(WriteSolutionToFile(da, U, filename));

        PetscCall(VecDestroy(&U));
        PetscCall(VecDestroy(&U_exact));
        PetscCall(TSDestroy(&ts));
        PetscCall(DMDestroy(&da));
    }

    PetscCall(PetscFinalize());
    PetscFunctionReturn(0);
}
