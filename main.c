#include "bmp8.h"
#include "bmp24.h"
#include <stdio.h>

//to launch gcc main.c bmp_8.c -o image_processing
//then ./image_processing


//little main to check if it works
int main() {
    int choice, filterChoice, run = 1, bright, thresh, flip;
    char filename[256], saveFilename[256], colored;
    t_bmp8 *image = NULL;
    t_bmp24 *imagec = NULL;


    //to verify if its a 8 or 24 deep
    printf("Is it a colored image ? : (Y/N) ");
    scanf(" %c", &colored);
    while (colored != 'Y' && colored != 'N') {
        printf("Invalid input. Please enter 'Y' or 'N': ");
        scanf(" %c", &colored); 
    }

    //biggest loop to run the program
    while (run) {

        switch (colored)
        {
            //Case for a gray bmp
            case 'N':

            printf("\nPlease choose an option:\n");
            printf("    1. Open an image\n");
            printf("    2. Save an image\n");
            printf("    3. Apply a filter\n");
            printf("    4. Display image information\n");
            printf("    5. Quit\n");
            printf("\n>>>>> Your choice: ");
            scanf(" %d", &choice);


            //switch case for better logic
            switch (choice) {
                case 1:
                    printf("Enter file path: ");
                    scanf("%255s", filename);

                    if (image) {
                        bmp8_free(image);
                    }
                    
                    image = bmp8_loadImage(filename);

                    
                    if (!image) {
                        printf("Failed to load image. Please check the file path.\n");
                        continue;
                    }
                    break;

                case 2:
                    if (!image) {
                        printf("No image loaded, can't save the image.\n");
                    } else {
                        printf("Enter save file path: ");
                        scanf("%255s", saveFilename);

                        bmp8_saveImage(saveFilename, image);
                        printf("Image saved successfully.\n");
                    }
                    break;

                    case 3:
                    if (!image) {
                        printf("No image loaded; can't apply a filter.\n");
                    } else {
                        printf("Choose a filter:\n");
                        printf("    1. Negative\n");
                        printf("    2. Brightness\n");
                        printf("    3. Black and white\n");
                        printf("    4. Box blur\n");
                        printf("    5. Gaussian blur\n");
                        printf("    6. Outline\n");
                        printf("    7. Emboss\n");
                        printf("    8. Return to previous menu\n");
                        printf(">>>>> Your choice: ");
                        scanf(" %d", &filterChoice);
                
                        switch (filterChoice) {
                            case 1:
                                bmp8_negative(image);
                                printf("Negative filter applied.\n");
                                break;
                
                            case 2:
                                printf("Enter brightness adjustment value (-100 to 100): ");
                                scanf("%d", &bright);
                                if (bright < -100 || bright > 100) {
                                    printf("Invalid brightness value. Please enter between -100 to 100.\n");
                                } else {
                                    bmp8_brightness(image, bright);
                                    printf("Brightness filter applied.\n");
                                }
                                break;
                
                            case 3:
                                printf("Enter threshold (0-255): ");
                                scanf("%d", &thresh);
                                if (thresh < 0 || thresh > 255) {
                                    printf("Invalid threshold value. Must be between 0 and 255.\n");
                                } else {
                                    bmp8_threshold(image, thresh);
                                    printf("Threshold filter applied.\n");
                                }
                                break;
                
                            case 4:
                                bmp8_boxblur(image);
                                printf("Box blur filter applied.\n");
                                break;
                
                            case 5:
                                bmp8_gaussian(image);
                                printf("Gaussian filter applied.\n");
                                break;
                
                            case 8: 
                                printf("Returning to main menu.\n");
                                break;
                
                            default:
                                printf("Invalid filter choice. Please choose a valid option.\n");
                                break;
                        }
                    }
                    break;

                case 4:
                    if (!image) {
                        printf("No image loaded, can't display info.\n");
                    } else {
                        bmp8_printInfo(image);
                    }
                    break;

                case 5:
                    run = 0;
                    if (image) {
                        bmp8_free(image);
                    }
                    printf("Exiting program.\n");
                    break;

                default:
                    printf("Invalid choice. Please enter a number between 1 and 5.\n");
                    break;
            }

            break;


            //Case for bmp24 
            case 'Y':

            //print the menu
            printf("\nPlease choose an option:\n");
            printf("    1. Open an image\n");
            printf("    2. Save an image\n");
            printf("    3. Apply a filter\n");
            printf("    4. Rotate\n");
            printf("    5. Quit\n");
            printf("\n>>>>> Your choice: ");
            scanf(" %d", &choice);


            switch (choice) {
            case 1:
                printf("Enter file path: ");
                scanf(" %255s", filename);

                if (imagec) {
                    bmp24_free(imagec); 
                } 

                imagec = bmp24_loadImage(filename);

                if (!imagec) {
                    printf("Failed to load image. Please check the file path.\n");
                }
                
            break;

            case 2:
                if (!imagec) {
                    printf("No image loaded, can't save the image.\n");
                } else {
                    printf("Enter save file path: ");
                    scanf(" %255s", saveFilename);

                    bmp24_saveImage(imagec, saveFilename);
                    printf("Image saved successfully.\n");
                }
                break;
            
            case 3:

            if (!imagec) {
                printf("No image loaded, can't apply a filter.\n");
            } else {
                printf("Choose a filter:\n");
                printf("    1. Negative\n");
                printf("    2. Brightness\n");
                printf("    3. Grayscale\n");
                printf("    4. Box blur\n");
                printf("    5. Gaussian blur\n");
                printf("    6. Outline\n");
                printf("    7. Emboss\n");
                printf("    8. Return to previous menu\n");
                printf(">>>>> Your choice: ");
                scanf("%d", &filterChoice);


                switch (filterChoice) {
                    case 1:
                        bmp24_negative(imagec);
                        printf("Negative filter applied.\n");
                        break;

                    case 2:
                        printf("Enter brightness adjustment value: ");
                        scanf("%d", &bright);

                        bmp24_brightness(imagec, bright);
                        printf("Brightness filter applied.\n");
                        break;

                    case 3:
                        printf("Enter grayscale: ");
                        scanf("%d", &thresh);

                        bmp24_grayscale(imagec);
                        printf("Threshold filter applied.\n");
                        break;

                    case 4:
                        bmp24_boxBlur(imagec);
                        printf("Box blur applied.\n");
                    break;
                    
                    case 5:
                        bmp24_gaussianBlur(imagec);
                        printf("Gaussian blur applied.\n");
                    break;

                    case 6:
                        bmp24_outline(imagec);
                        printf("Outline filter applied.\n");
                    break;

                    case 7:
                        bmp24_emboss(imagec);
                        printf("Emboss filter applied.\n");
                    break;

                    case 8:
                        printf("Returning to the previous menu...\n");
                        return 0;

                    default:
                        printf("Invalid filter choice.\n");
                        break;
                }
            }

            break;

            case 4:
                printf("Choose an option:\n");
                printf("    1. Rotate of 90 degree\n");
                printf("    2. Flip it horizontaly\n");
                printf("\n>>>>> Your choice: ");
                scanf("%d", &flip);

                switch (flip)
                {
                case 1: {
                    t_bmp24 *rotate = bmp24_rotate90c(imagec);
                    bmp24_free(imagec);
                    imagec = rotate;
                    break;
                }
                
                default:
                    break;
                }
            break;

            case 5:
                run = 0;
                if (imagec) {
                    bmp24_free(imagec);
                }
                printf("Exiting program \n");
            
            break;
            
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");

            }

            break;
        }
       
    }


    return 0;
}
