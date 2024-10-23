import sys

def count_set_bits(n):
    count = 0
    while n > 0:
        n &= (n - 1)
        count += 1
    return count

def turnOffK(n, k):
    return n if k <= 0 else (n & ~(1 << (k - 1)))

def autocorrelation(length):
    search_space = 1 << (length - 1)
    c, optimal, num_of_optimal = 0, 0, 0
    minenergy = float('inf')

    for count in range(0, search_space):
        energy = 0
        
        for i in range(1, length):
            r = length - i 
            p = 1 << r 
            d = count >> i 
            x = count^d 

            if x >= p:
                for j in range(length, r, -1):
                    x = turnOffK(x, j)
                    if x < p:
                        j = length - i + 1

            o = count_set_bits(x)
            z = r - o 
            c = z - o 
            energy += c * c 
            if energy >= minenergy:
                i = length - 1

        if minenergy > energy:
            minenergy = energy
            optimal = count
            num_of_optimal = 1
        elif energy == minenergy:
            num_of_optimal += 1
    
    num_of_optimal *= 2
    print(f"Min E = {minenergy}\nFirst Opt = {optimal}\nOptimal sequences = {num_of_optimal}")

if __name__ =='__main__':
    autocorrelation(int(sys.argv[1]))
