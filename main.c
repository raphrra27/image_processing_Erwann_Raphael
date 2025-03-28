 #include "bmp8.h"

//little main to chekc if its work
int main() {
    int choice, filterChoice, run = 1, bright, thresh;
    char filename[256], saveFilename[256];
    t_bmp8 *image = NULL;

    //Biggest loop to run the programme
    while (run) {
        printf("\nPlease choose an option:\n    1. Open an image\n    2. Save an image\n    3. Apply a filter\n    4. Display image information\n    5. Quit\n");
        printf("\n>>>>> Your choice: ");
        scanf("%d", &choice);

        //switch case because it is logical (more than while)
        switch (choice) {
            case 1:
                //To open a file
                printf("Enter file path: ");
                scanf("%255s", filename);
                image = bmp8_loadImage(filename);
                break;

            case 2:
                //To save an image
                if (!image) {
                    printf("No image loaded, can't save the image.\n");
                } else {
                    printf("Enter save file path: ");
                    scanf("%255s", saveFilename);
                    bmp8_saveImage(saveFilename, image);
                }
                break;

            case 3:
                //To save a filter
                if (!image) {
                    printf("No image loaded, can't apply a filter.\n");
                } else {
                    printf("Choose a filter:\n    1. Negative\n    2. Brightness\n    3. Black and white\n    4. Box blur\n    5. Gaussian blur\n    6. Outline\n    7. Emboss\n    8. Return to previous menu\n");
                    printf(">>>>> Your choice: ");
                    scanf("%d", &filterChoice);


                    switch (filterChoice) {
                        case 1: //For the negative filter
                            bmp8_negative(image);
                        printf("Negative filter applied.\n");
                        break;
                        case 2: //For the brightness filter
                            printf("Whats the value of the bright you wanna change ? ");
                        scanf("%d", &bright);

                        bmp8_brightness(image,bright);
                        printf("Brightness filter applied.\n");
                        break;
                        case 3: //For the treshold
                            printf("Which color of pixel you wanna bright ? (0-255) :");
                        scanf("%d", &thresh);

                        bmp8_threshold(image, thresh);
                        printf("Brightness filter applied.\n");
                    }
                }
                break;

            case 4:
                //Display all the information
                if (!image) {
                    printf("No image loaded, can't display info.\n");
                } else {
                    bmp8_printInfo(image);
                }
                break;

            case 5:
                //To exit the programme
                run = 0;
                if (image) {
                    bmp8_free(image);
                }
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
    return 0;
}

