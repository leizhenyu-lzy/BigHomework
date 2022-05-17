# 1950083 自动化 刘智宇
import numpy as np
import pandas as pd
import ReadDataset

def haha():
    pass


if __name__ == "__main__":
    melon_dataset, melon_index = ReadDataset.readDataset()
    (melon_dataset_rows, melon_dataset_cols) = melon_dataset.shape
    print(melon_dataset_rows, melon_dataset_cols)
    melon_properties, melon_label = ReadDataset.separateDataLabel(melon_dataset)
