#include "bmp8.h"
#include <stdio.h>

// To launch gcc main.c bmp_8.c -o image_processing
// Then ./image_processing


// Little main to check if it works
int main() {
    int choice, filterChoice, run = 1, bright, thresh;
    char filename[256], saveFilename[256];
    t_bmp8 *image = NULL;

    // Biggest loop to run the program
    while (run) {
        printf("\nPlease choose an option:\n");
        printf("    1. Open an image\n");
        printf("    2. Save an image\n");
        printf("    3. Apply a filter\n");
        printf("    4. Display image information\n");
        printf("    5. Quit\n");
        printf("\n>>>>> Your choice: ");
        scanf("%d", &choice);
        while(getchar() != '\n'); // Flush stdin

        // Switch case for better logic
        switch (choice) {
            case 1:
                printf("Enter file path: ");
                scanf("%255s", filename);
                while(getchar() != '\n'); // Flush stdin

                if (image) {
                    bmp8_free(image);  // Free previous image
                }

                image = bmp8_loadImage(filename);
                if (!image) {
                    printf("Failed to load image. Please check the file path.\n");
                }
                break;

            case 2:
                if (!image) {
                    printf("No image loaded, can't save the image.\n");
                } else {
                    printf("Enter save file path: ");
                    scanf("%255s", saveFilename);
                    while(getchar() != '\n'); // Flush stdin
                    bmp8_saveImage(saveFilename, image);
                    printf("Image saved successfully.\n");
                }
                break;

            case 3:
                if (!image) {
                    printf("No image loaded, can't apply a filter.\n");
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
                    scanf("%d", &filterChoice);
                    while(getchar() != '\n'); // Flush stdin

                    switch (filterChoice) {
                        case 1:
                            bmp8_negative(image);
                            printf("Negative filter applied.\n");
                            break;

                        case 2:
                            printf("Enter brightness adjustment value: ");
                            scanf("%d", &bright);
                            while(getchar() != '\n'); // Flush stdin
                            bmp8_brightness(image, bright);
                            printf("Brightness filter applied.\n");
                            break;

                        case 3:
                            printf("Enter threshold (0-255): ");
                            scanf("%d", &thresh);
                            while(getchar() != '\n'); // Flush stdin
                            bmp8_threshold(image, thresh);
                            printf("Threshold filter applied.\n");
                            break;

                        default:
                            printf("Invalid filter choice.\n");
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
        }
    }
    return 0;
}
