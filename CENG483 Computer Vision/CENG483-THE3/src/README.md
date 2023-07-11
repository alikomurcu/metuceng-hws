# CENG483 - THE3 
We created subdirectories into **src** directory which includes distinct python scripts. Directories; **part1** for baseline, **part2** for further experiments, **part3** for best configuration results. Although we provided all the relevant results in the report, to see the results of each part, you can run corresponding python file in each part.


Also, we provided the **estimation_test.npy** file in the **src** directory. This file includes the estimated values of randomly selected 100 images from 
test dataset. You can find the relative paths of these images in the **test_images.txt** file. To evaluate, you can put the test dataset into **test_dataset** directory 
under **src** directory.

Also, under the **src** directory, you will find a script named **test.py** along with the trained model **model.pt**. The **test.py** script loads the model (Note that we used CUDA while generating the model,
if the evaluation will be performed on a cpu-only machine, you should add the parameter "map_location=torch.device('cpu')" to the load function at line 57) and
evaluates the test dataset and creates **estimation_test.npy** file with **test_images** file that contains paths of the images that are used for evaluation.
In case of an error, you can recreate the **estimation_test.npy** file and **test_images.txt** file by running the **test.py** script. 

Furthermore, regarding the errors about the **model.pt** file, run part3/best_config.py script to recreate the model file. 





