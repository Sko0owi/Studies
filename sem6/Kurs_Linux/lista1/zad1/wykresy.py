import matplotlib.pyplot as plt
import numpy as np
import json


angle = 45


data = {}

with open("wyniki.txt", "r") as f:
    for x in f.readlines():
        data[x.split(":")[0]] = json.loads(":".join(x.split(":")[1:]))


Linux_Versions = []
Number_of_Files = []
Number_of_Lines_of_Code = []
Estimation_of_Effort = []
Estimation_of_Time = []
Estimation_of_Cost = []

for (linux_ver,json_dump) in data.items():
    NumberOfFiles = json_dump["SUM"]["nFiles"]
    NumberOfLines = json_dump["SUM"]["code"]
    EstimationOfEffort = (((NumberOfLines/1000) ** 1.05) * 2.4)
    EstimationOfTime = ((EstimationOfEffort ** 0.38) * 2.5) / 12
    EstimationOfCost = (EstimationOfEffort/12) * 2.4 * 56286



    Linux_Versions.append(linux_ver[6:])
    Number_of_Files.append(NumberOfFiles)
    Number_of_Lines_of_Code.append(NumberOfLines)
    Estimation_of_Effort.append(EstimationOfEffort)
    Estimation_of_Time.append(EstimationOfTime)
    Estimation_of_Cost.append(EstimationOfCost)

# print(Linux_Versions)
# print(Number_of_Files)
# print(Number_of_Lines_of_Code)
# print(Estimation_of_Effort)
# print(Estimation_of_Time)
# print(Estimation_of_Cost)

plt.figure(figsize=(20, 20))


plt.subplot(221)
plt.bar(Linux_Versions, Number_of_Files)
plt.title('Number of Files')
plt.xticks(rotation=angle, ha='right')

plt.subplot(222)
plt.bar(Linux_Versions, Number_of_Lines_of_Code)
plt.title('Number of Lines of Code')
plt.xticks(rotation=angle, ha='right')

plt.subplot(223)
plt.bar(Linux_Versions, Estimation_of_Time)
plt.title('Time to reproduce (years)')
plt.xticks(rotation=angle, ha='right')

plt.subplot(224)
plt.bar(Linux_Versions, Estimation_of_Cost)
plt.title('Cost to reproduce ($)')
plt.xticks(rotation=angle, ha='right')

plt.savefig('kernel_history.png')
plt.show()

