import numpy as np

text = "BREATHTAKING"

answer = "RUPOTENTOIFV"




alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
revMap = {}
for i,ch in enumerate(alphabet):
    revMap[ch] = i

wyznacznik = 1


vector = []
for i in text:
    vector.append(revMap[i])

ans_vector = []
for i in answer:
    ans_vector.append(revMap[i])


# start_matrix = [['a', 'b', 'c'], ['d', 'e', 'f'], ['g', 'h', 'i']]


# matrix_solve = [np.zeros((4,3), dtype=np.int64) for _ in range(3)]
# matrix_answer = [np.zeros((4), dtype=np.int64) for _ in range(3)]
# print(matrix_solve)
# for i in range(0,12,3):

#     for row,variables in enumerate(start_matrix):
#         first = True
#         for pog, variable in enumerate(variables):
#             if not first:
#                 print(" + ", end="")
#             first = False
#             matrix_solve[row][i//3][pog] = vector[i+pog] 
#             print(f"{vector[i+pog]}{variable}", end="")
#         matrix_answer[row][i//3] = ans_vector[i + row]
#         print(f" = {ans_vector[i + row]};")


# print(np.shape(matrix_solve[0]))
# print(np.shape(matrix_answer[0]))


# x = np.linalg.lstsq(matrix_solve[0], matrix_answer[0])
# print(x)
# exit()

vector = np.array(vector)
print(vector)

start_matrix = np.zeros((3,3), dtype=np.int64)

size = 3

columns = [[] for _ in range(size)]
for i in range(size):
    row = np.zeros(size, dtype=np.int64)
    for i1 in range(26):
        for i2 in range(26):
            for i3 in range(26):
                row[0] = i1
                row[1] = i2
                row[2] = i3
                
                good = True
                # for l in range(0,12,size):
                my_ans = np.inner(row,vector[:size])
                if(my_ans != ans_vector[i]):
                    good = False
                    
                if(good):
                    print(row)
                    columns[i].append((i,np.copy(row)))

                    
                
print(columns)
                

                

# print(start_matrix)
# print(vector[:2])
# print(start_matrix @ vector[:2])
# random_bullshit = (np.random.randint(26, size=(2,2)))
# while wyznacznik:

#     random_bullshit = (np.random.randint(26, size=(2,2)))
#     wyznacznik = np.linalg.det(random_bullshit)
#     # print(wyznacznik)
#     # print(random_bullshit)