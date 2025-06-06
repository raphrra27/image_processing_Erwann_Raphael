/////////////////HEADER////////////////
//Authors: Riera Raphael Briot Erwann//
//Description: file main.c so its regroup all the functions writes and allow to actually use the prgram//
//Role: its the main of the prog//
//Execution: 1. "gcc main.c bmp8.c bmp24.c -o image_processing" then 2. "./image_processing"

//INCLUDES NEEDED
#include "bmp8.h"
#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>



//MAIN
int main() {
    //variables needed in the programm
    int choice, filterChoice, run = 1, bright, thresh, flip;
    char filename[256], saveFilename[256], colored;
    t_bmp8 *image = NULL;
    t_bmp24 *imagec = NULL;


    //to verify if its a 8 or 24 deep (colored or not)
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
            //Case for a gray bmp (bmp8)
            case 'N':

            printf("\nPlease choose an option:\n");
            printf("    1. Open an image\n");
            printf("    2. Save an image\n");
            printf("    3. Apply a filter\n");
            printf("    4. Display image information\n");
            printf("    5. Quit\n");
            printf("\n>>>>> Your choice: ");
            scanf(" %d", &choice);


            //switch case for better logic (this is the menu)
            switch (choice) {
                //load image
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
                
                //save image
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
                
                //apply a filter in the image
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
                        printf("    8. Equalize\n");
                        printf("    9. Return to previous menu\n");
                        printf(">>>>> Your choice: ");
                        scanf(" %d", &filterChoice);
                        
                        //switch choice for the filter to be applied
                        switch (filterChoice) {
                            //negative filter
                            case 1:
                                bmp8_negative(image);
                                printf("Negative filter applied.\n");
                                break;
                            
                            //brghtness filter
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
                            
                            //threshold filter
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
                            
                            //boxe blur filter
                            case 4:
                                bmp8_boxblur(image);
                                printf("Box blur filter applied.\n");
                                break;
                            
                            //gaussian filter
                            case 5:
                                bmp8_gaussian(image);
                                printf("Gaussian filter applied.\n");
                                break;
                            
                            //outline filter
                            case 6:
                                bmp8_outline(image);
                                printf("Outline filter applied.\n");
                                break;
                            
                            //emboss filter
                            case 7:
                                bmp8_emboss(image);
                                printf("Emboss filter applied.\n");
                                break;
                                
                            //equalize filter
                            case 8:
                                unsigned int *histogram = bmp8_computeHistogram(image);
                                unsigned int *hist_eq = bmp8_computeCDF(histogram);
                                bmp8_equalize(image, hist_eq);
                                free(histogram);
                                free(hist_eq);
                                printf("Equalization filter applied.\n");
                                break;

                            //return to the previous menu
                            case 9: 
                                printf("Returning to main menu.\n");
                                break;
                            
                            //if choice is over 9
                            default:
                                printf("Invalid filter choice. Please choose a valid option.\n");
                                break;
                        }
                    }
                    break;
                
                //to display the information of the image bmp8
                case 4:
                    if (!image) {
                        printf("No image loaded, can't display info.\n");
                    } else {
                        bmp8_printInfo(image);
                    }
                    break;

                //to exit the prog
                case 5:
                    run = 0;
                    if (image) {
                        bmp8_free(image);
                    }
                    printf("Exiting program.\n");
                    break;
                
                //if hte choice is over 5
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
            printf("    4. Quit\n");
            printf("\n>>>>> Your choice: ");
            scanf(" %d", &choice);

            //switch for better logic for the menu choice
            switch (choice) {
            //load he image
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
            
            //to save the image
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
            
            //to apply a filter 
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

                //switch menu for the choice of the filter
                switch (filterChoice) {
                    //negative filter
                    case 1:
                        bmp24_negative(imagec);
                        printf("Negative filter applied.\n");
                        break;

                    //brightness filter
                    case 2:
                        printf("Enter brightness adjustment value: ");
                        scanf("%d", &bright);

                        bmp24_brightness(imagec, bright);
                        printf("Brightness filter applied.\n");
                        break;

                    //graysacle filter
                    case 3:
                        printf("Enter grayscale: ");
                        scanf("%d", &thresh);

                        bmp24_grayscale(imagec);
                        printf("Threshold filter applied.\n");
                        break;

                    //boxblur filter
                    case 4:
                        bmp24_boxBlur(imagec);
                        printf("Box blur applied.\n");
                    break;
                    
                    //gaussian blur filter
                    case 5:
                        bmp24_gaussianBlur(imagec);
                        printf("Gaussian blur applied.\n");
                    break;

                    //outline filter
                    case 6:
                        bmp24_outline(imagec);
                        printf("Outline filter applied.\n");
                    break;  

                    //emboss filter
                    case 7:
                        bmp24_emboss(imagec);
                        printf("Emboss filter applied.\n");
                    break;

                    //returning the the previous menu
                    case 8:
                        printf("Returning to the previous menu...\n");
                        return 0;

                    //if choice is over 9
                    default:
                        printf("Invalid filter choice.\n");
                        break;
                }
            }

            break;

            //to exit the prog
            case 4:
                run = 0;
                if (imagec) {
                    bmp24_free(imagec);
                }
                printf("Exiting program \n");
            
            break;
            
            //if choice is over 4
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");

            }

            break;
        }
       
    }


    return 0;
}