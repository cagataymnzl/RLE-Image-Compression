//PGM File RLE Encode and Decode Program
//by Çaðatay Menzil 18011018
//Irfan View is used to view PGM Files 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//RLE the PGM image---PGM dosyasýný sýkýþtýrma
int* rlencode(int row,int column,int image[][column]){
	FILE* encoded;
	encoded = fopen("encoded.txt", "wb");
	
    /* Eðer bütün deðerler farklý ise image in 2 katý kadar yer ayrýlmalýdýr.
	12 13 14 15 -> 1 12 1 13 1 14 1 15  */
    int* dest = (int*)malloc(sizeof(int) * (row * column * 2));
    int i=0,j=0,dest_index=0;
    int rLen=0,temp;
  	
    dest[dest_index++]=row;
    dest[dest_index++]=column;
    fprintf(encoded,"%d %d\n",row,column);
    /* traverse the input image one by one */
    temp=image[0][0];
	for(i=0;i<row;i++){
    	for(j=0;j<column;j++){
	    	if(temp!=image[i][j]){
	    		dest[dest_index++]=rLen;
				dest[dest_index++]=temp;
				fprintf(encoded,"%d ",rLen);
				fprintf(encoded,"%d ",temp);
				temp=image[i][j];
				rLen=1;
			}
			else{
				rLen++;
			}
		}	
	}
	dest[dest_index++]=rLen;
	dest[dest_index++]=temp;
	fprintf(encoded,"%d ",rLen);
	fprintf(encoded,"%d ",temp);
	fclose(encoded);

	for(i=0;i<dest_index;i++){
		printf("%d ",dest[i]);
	}
	printf("\n");
	return dest;
}

//sýkýþtýrýlmýþ dosyayý açma ve matris haline getirme
void decode(int source[]){
	int i,j,count,column_count=0;
	int source_index=2,space=0,sum=0;
	FILE* decoded_pgm;
	
	decoded_pgm = fopen("decoded_pgm.pgm", "wb");
	//Sihirli Sayý
	fprintf(decoded_pgm, "P2\n");
	// Sütun ve satýr deðerleri
	fprintf(decoded_pgm, "%d %d\n", source[0], source[1]);
	// Maximum deðer
	fprintf(decoded_pgm, "255\n");
	//---------KONTROLLER---------
	int control = 1;
	//Kaynak dosyasýndaki renklerin sayýsýnýn toplamý
	for(i=2;i<source[0]*source[1]*2;i+=2){
		sum+=source[i];
	}
	//Toplam satir*sutun sayýsýna eþit deðilse dosya açýlmaz
	if(sum==source[0]*source[1]){
		printf("This file cannot open-(because of sizes)!\n");
		control=0;
	}
	//0-255 renk aralýðý kontrolü
	i=3;
	while(i<source[0]*source[1]*2 && source[i]<256 ) {
		if(source[i]<256){
			i+=2;
		}
		else{
			control=0;
			printf("This file cannot open-(because of range of color)!\n");
		}
	}
	//Arka Arkaya Ayný Renk RLE Kontrolü
	for(i=3;i<source[0]*source[1]*2;i+=2){
		if(source[i]==source[i+2]){
			control=0;
			printf("This file wasn't encoded correctly!'");
		}
	}
	//En son bütün kontroller doðru ise dosyayý çözümle
	//decoded_pgm adlý yeni pgm dosyasýna yaz
	if(control==1){
		count=source[source_index];
		while(count>0&&space<source[0]*source[1]){
			fprintf(decoded_pgm,"%d ",source[source_index+1]);
			printf("%d ",source[source_index+1]);
			count--;
			column_count++;
			space++;
			if(column_count==source[1]){
				fprintf(decoded_pgm,"\n");
				printf("\n");
				column_count=0;
			}
			if(count==0){
				source_index+=2;
				count=source[source_index];
			}
		}
	}
	fclose(decoded_pgm);
}
//Verilen bir rengin deðerini deðiþtirme
int* change_acolor_byvalue(int source[]){
	int first,last,index;
	printf("Which exist color do you want to change in image?");
	scanf("%d",&first);
	printf("Select a new color for change:");
	scanf("%d",&last);
	if(last>0&&last<256){
		for(index=3;index<source[0]*source[1]*2;index+=2){
			if(first==source[index]){
				source[index]=last;
			}
		}
	}
	else{
		printf("This color(%d) is not in range(0-255)!\n",last);
	}
	return source;
}
//Koordinatlarý verilen rengi deðiþtirme
int* change_acolor_bylocation(int source[]){
	int row,column,source_index=2;
	int last,count;
	
	printf("Enter the location of pixel do you want to change!\n");
	printf("Row of pixel:");scanf("%d",&row);
	printf("Column of pixel:");scanf("%d",&column);
	printf("Enter the new value:");scanf("%d",&last);

	count=row*source[1]+column+1;
	while(count>source[source_index]&&source_index<source[0]*source[1]*2){
		count=count-source[source_index];
		source_index+=2;
	}
	printf("---%d converts to %d----\n",source[source_index+1],last);
	source[source_index+1]=last;
	return source;
}
//Histogram oluþturma
void histogram(int source[]){
	int count=0,i,j;
	int sumof_colors[256];
	for(i=0;i<256;i++){
		sumof_colors[i]=0;
	}
	printf("\nROW:%d---COLUMN:%d\n",source[0],source[1]);
	for(i=0;i<256;i++){
		for(j=3;j<source[0]*source[1]*2;j+=2){
			if(source[j]==i){
				count+=source[j-1];
			}
		}
		sumof_colors[i]=count;
		count=0;
	}
	int temp;
	for(i=0;i<256;i++){
		if(sumof_colors[i]!=0){
			printf("%d rengi-%d adet==> ",i,sumof_colors[i]);
			temp=sumof_colors[i];
			while(temp>0){
				printf("*");
				temp--;
			}
			printf("\n");
		}
	}
}
int main(){
	int i, j, temp = 0;
	int row , column ;
	// !! ÖRNEK RESÝMLER !!
	/*int image[13][13] = {
	{ 15, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
	{ 15, 15, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31},
	{ 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47},
	{ 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63},
	{ 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79},
	{ 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95 },
	{ 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111},
	{ 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127},
	{ 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143},
	{ 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159},
	{ 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175},
	{ 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191},
	{ 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207}
	};
	/*int image[13][13] = {
	{ 1, 90, 1, 1, 1, 90, 1, 90, 90, 90, 90, 90, 1 },
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 90, 90, 90, 90, 1, 1, 0, 90, 1, 1, 1 },
	{ 1, 90, 90, 90, 90, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 1, 1, 90, 1, 1, 1},
	{ 1, 90, 1, 1, 1, 90, 1, 90, 90, 90, 90, 90, 1}
	};*/
	printf("******PGM FILE ENCODE-DECODE PROGRAM******\n");
	printf("Row Number:");scanf("%d",&row);
	printf("Column Number:");scanf("%d",&column);
	int image[row][column];
	for(i=0;i<row;i++){
		for(j=0;j<column;j++){
			printf("%d.row-%d.column element:",i,j);
			scanf("%d",&image[i][j]);
		}
		printf("\n");
	}
	FILE* pgmimg;
	pgmimg = fopen("pgmimg.pgm", "wb");

	// Sihirli Sayý
	fprintf(pgmimg, "P2\n");
	// Sütun ve satýr deðerleri
	fprintf(pgmimg, "%d %d\n", row, column);
	// Maximum deðer
	fprintf(pgmimg, "255\n");
	
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			temp = image[i][j];

			// Writing the gray values in the 2D array to the file
			fprintf(pgmimg, "%d ", temp);
		}
		fprintf(pgmimg, "\n");
	}
	int select;
	int* encoded;
	do{
		printf("\n(1)Run Length Encoding the PGM File\n");
		printf("(2)Decode the Source Array\n");
		printf("(3)Change a color by value\n");
		printf("(4)Change a color by location\n");
		printf("(5)Histogram\n");
		printf("(0)Exit\n");
		printf("***************************************\n");
		printf("Please select an Operation:");
		scanf("%d",&select);
		switch(select){
			case 1:
				encoded=rlencode(row,column,image);
				printf("\n\n");
				break;
			case 2:	
				decode(encoded);
				printf("\n\n");
				break;
			case 3:
				change_acolor_byvalue(encoded);
				break;
			case 4:
				change_acolor_bylocation(encoded);	
				break;
			case 5:
				printf("\nHISTOGRAM OF IMAGE");
				histogram(encoded);
				break;
		}	
	}while(select!=0);
	printf("\nProgram Finished---Files Created!");
	fclose(pgmimg);
	
	return 0;
}


