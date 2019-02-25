#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float** readPCAP(int* vsize) {

	//Open the file as a binary file, read-only.
	FILE* f = fopen("C:\\Users\\Michael\\Desktop\\VELODYNE\\VLP-16 Sample Data\\2015-07-23-15-08-34_Velodyne-VLP-16-Data_Depot 10Hz Single Returns.pcap", "rb");

	//Seek to the end of the file
	fseek(f, 0, SEEK_END); 
	int size = ftell(f); //Retreive and store the size of the file
	rewind(f); //Undo that seek

	//Prepare the array for storing the file data
	unsigned char* pcap_buffer = (unsigned char*)calloc(size + 1, sizeof(unsigned char));
	int size2 = fread_s(pcap_buffer, size + 1, sizeof(unsigned char), size, f); //Read the file data into the pcap file buffer

	fclose(f); //We dont need the file anymore so we can close it and free some memory

	if (size2 != size) {
		//If there was an error reading our file then return a null pointer
		*vsize = -1; //Return -1 to the outparameter as an indication of an error
		return 0;
	}


	//Packet counter and current run data
	int packets = 0;
	int run = 1;

	//The current angle of the packet (In the form of an unsigned short int (uint_16t))
	unsigned short int azimuth = 0;
	//The angle in a real value decimal number
	float angle = 0;

	//The vertex array data
	float** vertices = 0;

	//Initial scan to find out how many actual packets there are in the file.
	for (int i = 0; i < size; i++) {
		if (pcap_buffer[i] == 0xff) {
			if (pcap_buffer[i + 1] == 0xee) {
				packets++;
				continue;
			}
		}
	}

	//Allocate 32 times the number of packets (2 sets of 16 channels of data)
	vertices = (float**)calloc(packets * 32, sizeof(float*));
	packets = 0; //Reset the packet variable

	//Secondary scan to find and store the parsed data
	for (int i = 0; i < size; i++) {
		//Look for header 0xFFEE
		if (pcap_buffer[i] == 0xff) {
			if (pcap_buffer[i + 1] == 0xee) {
				run = 0;//Reset the run variable
				i++;//Skip to the 0xEE
				continue; //Finally, skip the 0xEE and perform no further processing
			}
		}

		//When the run is started, obtain the angular value of the packet
		//The angle is represented as two bytes and the value given is 100x larger than the actual angle (Values between 0 and 35999)
		if (run == 0) {
			azimuth = (unsigned short int)(((unsigned short int)pcap_buffer[i + 1]) << 8 | pcap_buffer[i]); //Perform the calculation
			angle = (azimuth / 100.0f) * (2 * 3.14 / 180.0); //Convert it to radians for the vector position calculation
		}
		else if (run == 2) {
			int channel = 0; //Each channel of data represents 2 degrees ( (+/- 8) degrees )
			int j;
			for (j = i; j < i + 96; j += 3) {
				unsigned short int distance = (((unsigned short int)pcap_buffer[j + 1]) << 8 | pcap_buffer[j]) * 2; //Retreive the distance which is the first two bytes of every three bytes
				unsigned char intensity = pcap_buffer[j + 2]; //The intensity is the third byte !!!(This data value hasnt been implemented yet because only the points are retrieved for a VBO store to improve rendering performance)!!!

				//Only retrive the useful data points
				if (distance > 200 && distance < 100000) {

					//Find out how many useful data points there are
					packets++;

					//Find out the second angle
					float angle2 = ((channel - 8.0f) * 4.0f * 3.14f) / 180.0f;

					//Perform vertex calculations for each element
					float* vertex = (float*)calloc(3, sizeof(float));
					vertex[0] = distance * (cos(angle2) * sin(angle)); //x
					vertex[1] = distance * (cos(angle2) * cos(angle)); //y
					vertex[2] = distance * (sin(angle2));			   //z

					vertices[packets - 1] = vertex;

				}
				channel++;
			}
			//Continue our main loop from where we left off
			i = j;
			//printf("%i/%i\n", i, size);
		}
		run++;


	}
	//Reallocate to the smaller array size (Shrinking an array with realloc is super fast compared to a lossless malloc)
	vertices = (float**)realloc(vertices, packets * sizeof(float*));

	//Free the string because its huge and takes up about 1000MB of memory
	free(pcap_buffer);

	//Set our out parameter for the size and return the vertices we found
	*vsize = packets;
	return vertices;
}
