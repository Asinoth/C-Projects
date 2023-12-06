#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs457_crypto.h>





void Caesar_crypto(char* plaintext, unsigned int key)
{
	int i;
	char ch = plaintext[0];
	int index;
	signed int pos;

	
		for (i = 0; plaintext[i] != '\0'; i++)
		{
			ch = plaintext[i];

			if (ch > 47 && ch <= 122)
			{		
			
				index = key % 122;
				pos = ch + index;

				if (pos > 122)
				{
					pos = pos - 122;

					ch = 47 + pos;

				}

				else
					ch = pos;


				plaintext[i] = ch;

			}

		}

		printf("Caesar Encrypted plaintext: %s\n", plaintext);

}


void Caesar_decrypto(char *plaintext, unsigned int key)
{
	int i;
	char ch = "";
	int index;
	signed int pos;

	for (i = 0; plaintext[i] != '\0'; i++)
	{
		ch = plaintext[i];

		if (ch > 47 && ch <= 122)
		{
			
			index = key % 122;
			pos = ch - index;

			if (pos < 47)
			{
				pos = ch - 47;
				pos = index - pos;

				ch = 122 - pos;

			}

			else
				ch = pos;


			plaintext[i] = ch;
		}

	}
	printf("Caesar Decrypted plaintext: %s", plaintext);



}



void OTP_crypto(char* plaintext, char* key, int size) //WARNING: OTP IS BYTE PERFORMED SO CHANGES IN PLAINTEXT STACK//
{
	char* OTP_Crypted = plaintext;


	for (int i = 0; i < size; i++)
		OTP_Crypted[i] = plaintext[i] ^ key[i];






	printf("\nEncrypted plaintext in OTP:");
	for (int i = 0; i < size; i++)
		printf("%c", OTP_Crypted[i]);

}




void OTP_decrypto(char* plaintext, char* key, int size) //WARNING: OTP IS BYTE PERFORMED SO CHANGES IN PLAINTEXT STACK//

{
	char* OTP_DeCrypted = plaintext;


	for (int i = 0; i < size; i++)
		OTP_DeCrypted[i] = plaintext[i] ^ key[i];






	printf("\nDecrypted plaintext in OTP:");
	for (int i = 0; i < size; i++)
		printf("%c", OTP_DeCrypted[i]);

}



void Aff_crypt(char* plaintext, int size)
{
	int a = 11;
	int b = 19;
	int m = 26;

	char* Aff_Crypted = plaintext;
	int index = 0;

	char alphabet[] =" ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < size-1; i++)
	{
		for (int j = 0; j <= 27; j++)
		{
			if (plaintext[i] == alphabet[j])
			{
				printf("\nTHE CHARACTER IS: %c\n", plaintext[i]);
				Aff_Crypted[i] = j;
				break;
			}
		}		
	}

	printf("\nNonencrypted plaintext in Affine: ");

	for (int i = 0; i < size - 1; i++)
		printf("%d", Aff_Crypted[i]);

	//crypting//
	for (int i = 0; i < size-1; i++)
		Aff_Crypted[i] = (a * Aff_Crypted[i] + b) % m;


	printf("\nEncrypted plaintext in Affine: ");
	for (int i = 0; i < size-1; i++)
		printf("%d", Aff_Crypted[i]);
	
}



void Aff_decrypt(char* plaintext, int size)
{
	int a = 11;
	int b = 19;
	int m = 26;

	//decrypting, THERE IS PROBABLY MATH ERROR...
	for (int i = 0; i < size - 1; i++)
		plaintext[i] = (a ^ -1) * ( plaintext[i] - b) % m;


	printf("\nDecrypted plaintext in Affine: ");
	for (int i = 0; i < size - 1; i++)
		printf("%d", plaintext[i]);

}



char* gen_random(char* s, const int len) ////HELPER FUNCTION TO GENERATE OTP KEY//
{
	static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = 0;
	return s;
}



void upper(char* string, int size)
{

	for (int i = 0; i < size; i++)
		string[i] = toupper(string[i]);


}



int main()
{


	unsigned int key = rand();
	printf("Key being used: %d\n", key);
	char* plaintext;
	char* Aff_plaintext;
	int size = 1;
	int Aff_size = 1;
	char Specials[] = "!@#$%^&*()_+{}:";
	char Numbers[] = "0123456789";

	plaintext = (char*)malloc(size);

	Aff_plaintext = (char*)malloc(Aff_size);


	if (plaintext == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}

	int choose = 0;

	printf("Type 0: testfile\n");
	printf("Type 1: user input\n");
	scanf("%d",&choose);
	getchar();




	if (choose == 1)
	{

		printf("Type plaintext: ");

		char c = 1;
		int special;
		int is_num;
		
		while (c)
		{
			c = getchar();
			if (c == 10)
				break;



			special = 0;
			is_num = 0;

			for (int i = 0; i < 16; i++)
			{
				if (c == Specials[i])
					special = 1;

				if (c == Numbers[i])
					is_num = 1;
			}

			if (special == 0)
			{
				plaintext[size - 1] = c;
				size++;
				plaintext = (char*)realloc(plaintext, size);


				if (is_num == 0)
				{

					Aff_plaintext[Aff_size - 1] = c;
					Aff_size++;
					Aff_plaintext = (char*)realloc(Aff_plaintext, Aff_size);
				}
			}


		}

		plaintext[size] = plaintext[size-1] = '\0';

	}

	else if (choose == 0)
	{
		FILE* fp;


		if ((fp = fopen("testfile.txt", "r")) == NULL) {
			printf("Error! opening file");

			// Program exits if the file pointer returns NULL.
			exit(1);
		}

		char c = 1;
		int special;
		int is_num;

		while (c)
		{
			c = fgetc(fp);
			if (c == EOF)
				break;



			special = 0;
			is_num = 0;

			for (int i = 0; i < 16; i++)
			{
				if (c == Specials[i])
					special = 1;

				if (c == Numbers[i])
					is_num = 1;
			}

			if (special == 0)
			{
				plaintext[size - 1] = c;
				size++;
				plaintext = (char*)realloc(plaintext, size);


				if (is_num == 0)
				{
								
					Aff_plaintext[Aff_size - 1] = c;
					Aff_size++;
					Aff_plaintext = (char*)realloc(Aff_plaintext, Aff_size);
				}
			}

			


		}




		plaintext[size] = plaintext[size - 1] = '\0';
		Aff_plaintext[Aff_size] = Aff_plaintext[Aff_size - 1] = '\0';


		fclose(fp);

	}

	else
	{
		printf("\nWrong choice friend...");
		return -1;

	}


	Caesar_crypto(plaintext,key);
	Caesar_decrypto(plaintext, key);



	char* OTP_S;
	OTP_S = (char*)malloc(size * sizeof(char));
	OTP_S = gen_random(OTP_S, size);

	printf("\nOTP Key is: %s", OTP_S);
	OTP_crypto(plaintext, OTP_S, size);
	OTP_decrypto(plaintext, OTP_S, size);

	upper(Aff_plaintext, Aff_size);

	Aff_crypt(Aff_plaintext, Aff_size);

	printf("\nEncrypted plaintext going into decrypt: "); //As it should
	for (int i = 0; i < Aff_size - 1; i++)
		printf("%d", Aff_plaintext[i]);

	Aff_decrypt(Aff_plaintext, Aff_size);


}


