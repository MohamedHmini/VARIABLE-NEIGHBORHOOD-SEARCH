import csv
import random


def read_ds(file_path = '', delimiter = ','):
    
    r = {}
    
    with open(file_path) as ds:
        csv_reader = csv.reader(ds, delimiter = delimiter)
        i = 0
        
        for row in csv_reader:
            if i == 0:
                r["n"] = int(row[0])
                r["m"] = int(row[1])
            elif i == 1:
                r["p"] = list(map(int, row))
            else:
                r["s"] = list(map(int, row))
            i += 1
            pass
    
    pass

    return r


ds = read_ds(input("PLEASE PROVIDE A DATASET PATH :"), delimiter = "\t")


# SOLVING A K-CARDINALITY PROBLEM : 
# ESSENTIAL FUNCTIONS : 
######################



# OBJECTIVE FUNCTION : 
def f(policy):
    loader = 0
    Machines = [0 for i in range(ds["m"])] 
    
    for i in policy: 
        mnv = min(Machines)
        mni = Machines.index(mnv)
        starting_pt = max(mnv, loader)
        
        loader = (starting_pt + ds["s"][i])
        Machines[mni] = (loader + ds["p"][i])
    
    return max(Machines)


# NEIGHBORHOOD CREATION FUNCTION : 
def __block_swapping(x, block_size = 1, swap_factor = 0, index = 0):
    lb = index+block_size + swap_factor;
    ub = index+2*block_size + swap_factor;
    
    xc = x.copy()
    xc[index:index+block_size], xc[lb:ub] = xc[lb:ub], xc[index:index+block_size]

    return xc

def __block_reversing(x, block_size = 1, index = 0):
    xc = x.copy()
    rb = xc[index:index + block_size]
    xc[index:index + block_size] = rb[::-1]
    return xc

def block_swapping(x, block_size = 1, swap_factor = 0, index = -1):
    bound = len(x) - (2*block_size + swap_factor) + 1;
    xc = None
    
    if index == -1 :
        index = random.randint(0, bound)
    
       
    if index < bound:
        xc = __block_swapping(x, block_size, swap_factor, index);

    return xc

def block_reversing(x, block_size = 1, index = -1):
    bound = len(x) - block_size + 1
    xc = None
    
    if index == -1 :
        index = random.randint(0, bound)
    
       
    if index < bound:
        xc = __block_reversing(x, block_size, index);

    return xc


# CHANGING THE NEIGHBORHOOD OF A SPECIFIC SOLUTION : 
def change_neighborhood(x, bx, k):
    if f(x) < f(bx):
        bx = x
        k = 0
    else:
        k += 1
    return bx,k



# LOCAL SEARCH ALGO : 
#####################

# first improvement : 
def first_improvement(bx, struct):

    while True:
        x = bx
        i = 0

        while True:
            xc = struct(bx, i)
            if xc == None:
                break;
                
            if f(xc) < f(x):
                x = xc
                break; 
            else:
                i += 1
                
        if f(x) >= f(bx):
            break;
        else:
            bx = x
        
            
    return bx


def stochastic_hill_climbing(bx, struct, stopping_condition = 10):
    counter = stopping_condition
    
    while counter > 0:
        x = struct(bx, -1)
        if x is not None:
            if f(x) < f(bx):
                bx = x
                counter = stopping_condition
            else:
                counter -= 1
    
    return bx

# variable neighborhood descent : 
def VND(bx, Ns):
    k = 0
    while k < len(Ns):
        x = stochastic_hill_climbing(bx, Ns[k]) # local search
        print(f(x))
        bx,k = change_neighborhood(x, bx, k)
    return bx


# RVNS (N1) + VND (N2) :
def GVNS(bx, N1, N2, stopping_condition = 10):
    counter = stopping_condition
    
    while counter > 0:
        print(f(bx))
        x_start = bx.copy()
        k = 0
        while k < len(N1):
            x = N1[k](bx, -1) 
            x = VND(x, N2)
            bx, k = change_neighborhood(x, bx, k)
            
        if f(x_start) <= f(bx):
            counter -= 1
        else:
            counter = stopping_condition
    return bx


def ns1(x, i):
    return block_swapping(x, 10, 0, i)
def ns2(x, i):
    return block_swapping(x, 4, 30, i)
def ns3(x, i):
    return block_swapping(x, 1, 100, i)
def ns4(x, i):
    return block_reversing(x, 20, i)
def ns5(x, i):
    return block_reversing(x, 30, i)
def ns6(x, i):
    return block_reversing(x, 10, i)

N1 = [ns1, ns2, ns3]
N2 = [ns4, ns5, ns6]
bx = GVNS(list(range(len(ds["p"]))), N2, N1, stopping_condition=int(input("STOPPING CONDITION VALUE (allowed failure times) : ")))
print("GVNS : ")
print("OPTIMAL POLICY : {}".format(bx))
print("OPTIMAL VALUE : {}".format(f(bx)))