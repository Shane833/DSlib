# To analyze the distribution of the various algorithms
import pandas as pd

# Since I'll be calling this from a directory up
# Therefore I have set the path to be this
# which will loop inside the test directory 
df = pd.read_csv("test/test.csv")

# describe() provides an overview of the data
print(df.describe())