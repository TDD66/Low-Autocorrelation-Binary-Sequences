import array


N = 13
S = array.array('f', [1,1,1,1,1,-1,-1,1,1,-1,1,-1,1])
C = array.array('f', [0,0,0,0,0,0,0,0,0,0,0,0])
Y = array.array('f', [0,0,0,0,0,0,0,0,0,0,0,0])

for k in range (1,N):

    for i in range (0, N-k):
        C[k-1] += S[i]*S[i+k]


print(" ", C)

for j in range (0,N-1):
    Y[j] = C[j]**2

E = sum(Y)    

M = (N**2)/(2*E)

print(E)
print(M)
