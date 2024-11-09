import gzip
import numpy as np
import matplotlib.pyplot as plt
import idx2numpy
import csv

# def test() :
print ("supp")
file = 'data/train-images-idx3-ubyte'
file2 = 'data/train-labels-idx1-ubyte'

arr = idx2numpy.convert_from_file(file)
arr2 = idx2numpy.convert_from_file(file2)

with open('img.csv', 'w') as csvfile:
    imgwriter = csv.writer(csvfile)
    n = len(arr[0])
    
    for i in range(3):
        for j in range(n):
            imgwriter.writerow(arr[i][j])
        # print(i)
        # imgwriter.writerow([i])

with open('label.csv', 'w') as csvfile:
    labwriter = csv.writer(csvfile)
    n = len(arr2)    
    labwriter.writerow(arr2)


## To check the images.
# n = len(arr) / 10000
# for i in range( int(n)) :
#     plt.imshow(arr[i], cmap='gray')
#     plt.axis('off') 
#     plt.show()

