import gzip
import numpy as np
import matplotlib.pyplot as plt
import idx2numpy
import csv

# def test() :
print ("supp")
file = 'data/train-images-idx3-ubyte'
arr = idx2numpy.convert_from_file(file)

with open('img.csv', 'w') as csvfile:
    imgwriter = csv.writer(csvfile)
    n = len(arr[0])
    print(n)
    for i in range(6):
        for j in range(n):
            imgwriter.writerow(arr[i][j])
        # imgwriter.writerow("")


## To check the images.
# n = len(arr) / 10000
# for i in range( int(n)) :
#     plt.imshow(arr[i], cmap='gray')
#     plt.axis('off') 
#     plt.show()

