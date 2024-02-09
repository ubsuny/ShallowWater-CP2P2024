# Use an official Ubuntu base image
FROM ubuntu:20.04

# Avoid prompts from apt during build
ARG DEBIAN_FRONTEND=noninteractive

# Install basic tools, Fortran, and PETSc dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    wget \
    python3 \
    python3-dev \
    python3-pip \
    libblas-dev \
    liblapack-dev \
    libopenmpi-dev \
    gfortran \
    && rm -rf /var/lib/apt/lists/*

# Set a working directory (optional)
WORKDIR /opt

# Clone PETSc repo
RUN git clone -b release https://gitlab.com/petsc/petsc.git petsc

# Set PETSc environment variables
ENV PETSC_DIR=/opt/petsc PETSC_ARCH=arch-linux-c-opt

# Configure and build PETSc
RUN cd petsc && \
    ./configure --with-cc=mpicc --with-cxx=mpicxx --with-fc=mpif90 --download-fblaslapack --download-mpich --with-debugging=0 COPTFLAGS='-O3 -march=native' CXXOPTFLAGS='-O3 -march=native' FOPTFLAGS='-O3 -march=native' && \
    make all

# Set the default command for the container (optional)
CMD ["/bin/bash"]
