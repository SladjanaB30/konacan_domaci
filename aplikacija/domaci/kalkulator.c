#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *fp;

int ispisi_rezultat()
{
	fp=fopen("/dev/alu","r");

	if(fp==NULL)
	{
		printf("Greska pri otvaranju");
		return -1;
	
	}
	int rezultat,carry;
	fscanf(fp,"%d , %d",&rezultat,&carry);
	fclose(fp);
	if(rezultat<=255 && rezultat>=0)
		printf("Resenje je %d\n",rezultat);
	else 
		printf("Resenje je van opsega 0-255\n");
	return 0;

}

int operacija(char regx,char regy,char op)
{
	char buff[30];
	fp=fopen("/dev/alu","w");
	sprintf(buff,"reg%c %c reg%c",regx,op,regy);
	fputs(buff,fp);
	fclose(fp);

	fp=fopen("/dev/alu","r");
	int rezultat,carry;
	fscanf(fp,"%d , %d", &rezultat,&carry);
	fclose(fp);

	fp=fopen("/dev/alu","w");
	sprintf(buff,"reg%c=%x",regx,rezultat);
	fputs(buff,fp);
	fclose(fp);

}

int main()
{
	char buff[30];

	while(1)
	{
		printf("Unesite izraz: ");
		scanf("%s",buff);
		if(!strcmp(buff,"exit"))
			return 0;

		int val[4];
		char op[3];

		int i;
		for(i=0;i<4;i++)
			val[i]=0;
		for(i=0;i<3;i++)
			op[i]=0;
		

		sscanf(buff,"%d%c%d%c%d%c%d",&val[0],&op[0],&val[1],&op[1],&val[2],&op[2],&val[3]);
		

	        for(i=0;i<4;i++)
		{
			fp=fopen("/dev/alu","w");
			sprintf(buff,"reg%c=%x",'A'+i,val[i]);
			fputs(buff,fp);
			fclose(fp);
		
		}

	        int broj_operacija;
		if(op[0]!=0) broj_operacija=1;
		if(op[1]!=0) broj_operacija=2;
		if(op[2]!=0) broj_operacija=3;

		if(broj_operacija==1)
			operacija('A','B',op[0]);
		else if(broj_operacija==2)
		{
			if( (op[1]=='*' || op[1]=='/') && (op[0]=='+' || op[0]=='-') )
			{
				operacija('B','C',op[1]);
				operacija('A','B',op[0]);
			}
			
			else
			{
				operacija('A','B',op[0]);
				operacija('A','C',op[1]);
			}
		
		}
	      	else if(broj_operacija==3)
		{
			int brojac=0;
			for(i=0;i<3;i++)
				if(op[i]=='*' || op[i]=='/')
					brojac++;
			if(brojac==0 || brojac==3)
			{
				operacija('A','B',op[0]);
				operacija('A','C',op[1]);
				operacija('A','D',op[2]);

			}
			else if(brojac==1)
			{
				if(op[0]=='*' || op[0]=='/')
				{
					operacija('A','B',op[0]);
					operacija('A','C',op[1]);
					operacija('A','D',op[2]);

				}
				if(op[1]=='*' || op[1]=='/')
				{
					operacija('B','C',op[1]);
					operacija('A','B',op[0]);
					operacija('A','D',op[2]);

				}
	
				if(op[2]=='*' || op[2]=='/')
				{	
					operacija('C','D',op[2]);
					operacija('A','B',op[0]);
					operacija('A','C',op[1]);

				}
				
			}
			else if(brojac==2)
			{
				if( (op[0]=='*' || op[0]=='/') && (op[1]=='*' || op[1]=='/') )
				{
					operacija('A','B',op[0]);
					operacija('A','C',op[1]);
					operacija('A','D',op[2]);
				}
				if( (op[0]=='*' || op[0]=='/') && (op[2]=='*' || op[2]=='/') )
				{
					operacija('A','B',op[0]);
					operacija('C','D',op[2]);
					operacija('A','C',op[1]);
				}
				if( (op[1]=='*' || op[1]=='/') && (op[2]=='*' || op[2]=='/') )
				{
					operacija('B','C',op[1]);
					operacija('B','D',op[2]);
					operacija('A','B',op[0]);
				}

			}	
		}
		ispisi_rezultat();

	}
}
