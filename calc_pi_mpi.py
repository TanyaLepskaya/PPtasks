from mpi4py import MPI
import numpy
import random

# Вычисляем кол-во точек внутри круга
def compute_pi(samples_num):
    count = 0
    i = 0
    while i < samples_num:
        x = random.random()
        y = random.random()
        if x**2 + y**2 <= 1:
            count += 1
        i = i + 1
    return count

comm = MPI.COMM_WORLD
nprocs = comm.Get_size()
myrank = comm.Get_rank()

# Всеобщая рассылка параметра samples_num
if myrank == 0:
    N = 10000000 // nprocs
    samples_num = N
else:
    samples_num = None
samples_num = comm.bcast(samples_num, root=0)

comm.Barrier()
start_time = MPI.Wtime()

# Параллельное вычисление
mypi = compute_pi(samples_num) / nprocs

# Объядинение всех параллельныъ вычислений
pi_c = comm.reduce(mypi, op=MPI.SUM, root=0)

comm.Barrier()

# Расчет хначения Pi
pi = 4*float(pi_c)/N
if myrank == 0:
    error = abs(pi - numpy.pi)
    end_time = MPI.Wtime()
    print("pi is approximately %.16f, error is %.16f" % (pi, error))
    print("Elapsed time:", end_time - start_time)