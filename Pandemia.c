#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include<time.h>

typedef struct enfermera
{
 int id_enfermera;
 char nombre[20];
 char app[20];
 char apm[20];
 char telefono[12];
 char direccion[40];
 char codpostal[20];
 int edad ;

}Enfermera;

typedef struct paciente
{
 int id_paciente;
 char nombre[20];
 char app[20];
 char apm[20];
 char estado;
 char genero;
 int edad;
 
}Paciente;

typedef struct respirador
{
 int id_respirador;
 int id_paciente;
 char modelo[15];
}Respirador;

typedef struct cama
{
 int id_cama;
 int id_enfermera;
 int id_paciente;
}Cama;

//SYSTEMA
void BorrarPantalla()
{
 	system("cls");	
}

int  GeneraID()
{	int id;
	srand(time(NULL));
	id=1 + rand() % 200;
	return id;
}




void OrdenarResPac(Paciente *p,int nres)
{
 int i,j,x;            
 Paciente temp;
 Paciente temp2;
	 for (i=1;i<nres;i++)
		{
       		for (j=0; j < nres-i ;j++) 
       		{
          	 if ((p+j)->edad <(p+j+1)->edad)
          	 	{
            	 temp=*(p+j);
            	 temp2=*(p+j+1);
            	 *(p+j)=temp2;
            	 *(p+j+1)=temp;
          		}
       		}
		}
	}


int contarCamasDeEnfermera(FILE *flujo,int id_enfermera,int band)
{
	Cama cama;
    fread(&cama,sizeof(Cama),1,flujo);
	if(!feof(flujo))
 	{
 		if(cama.id_enfermera==id_enfermera && cama.id_paciente==0)
 		{
 			band++;
 		} 	 	
 	 	return contarCamasDeEnfermera(flujo,id_enfermera,band);
 	}
 return band; 	
}

void RecorrerArrayEnfermeras(Enfermera *p,int numenfermeras,int contador)
{
 FILE *flujo;
 int n;
	if(contador<numenfermeras)
	{
		flujo=fopen("Camas.bin","rb");
		n=contarCamasDeEnfermera(flujo,(p+contador)->id_enfermera,0);
		if(n>0)
		{
		printf(" %i ",(p+contador)->id_enfermera );
		printf(" %s ",(p+contador)->nombre );
		printf(" %s",(p+contador)->app );
	    printf(" %s",(p+contador)->apm );
		printf("-- %i |\n",n );
        }
        fclose(flujo);
		contador++;    
		return RecorrerArrayEnfermeras(p,numenfermeras,contador); 
	}
	return;
}


void OrdenarEnfermeras(Enfermera *p,int numenfermeras)
{
 int i,j,x;     
 int nc1;
 int nc2;   
 FILE *flujo;    
 Enfermera temp;
 Enfermera temp2;
	for (i=1;i<numenfermeras;i++)
	{
       		for (j=0; j < numenfermeras-i ;j++) 
       		{
       		flujo=fopen("Camas.bin","rb");
			nc1=contarCamasDeEnfermera(flujo,(p+j)->id_enfermera,0);
			fclose(flujo);
			flujo=fopen("Camas.bin","rb");
			nc2=contarCamasDeEnfermera(flujo,(p+j+1)->id_enfermera,0);
			fclose(flujo);
          	if (nc1 > nc2)
          	{
            	temp=*(p+j);
            	temp2=*(p+j+1);
            	*(p+j)=temp2;
            	*(p+j+1)=temp;
          	}
       		
       	}
	}
}

//FUNCIONES PACIENTE/RESPIRADOR
Paciente BuscarPacienteID(FILE *flujo,int id_paciente,Paciente pac)
{
    Paciente paciente;
    fread(&paciente,sizeof(Paciente),1,flujo); 
	if(!feof(flujo))
 	{	
 		if(paciente.id_paciente==id_paciente)
 		{
 		    pac=paciente;
 			return pac;
 		}
 	 	return BuscarPacienteID(flujo,id_paciente,pac);
 	}

 return pac; 	
}

void PacientesConRespiradores(FILE *flujo2,Paciente *p,int contador)
{
	FILE *flujo=fopen("Pacientes.bin","rb");
    Respirador respirador;
    Paciente pac;
    Paciente paciente;
    fread(&respirador,sizeof(Respirador),1,flujo2);
	if(!feof(flujo2))
 	{	  
  		pac=BuscarPacienteID(flujo,respirador.id_paciente,paciente);
       *(p+contador)=pac;
        contador++;
        fclose(flujo);
 	 	return PacientesConRespiradores(flujo2,p ,contador);
 	}
 fclose(flujo);
 return ; 	
}


void ExtraerEnfermeras(FILE *flujo,Enfermera *p,int contador)
{
    Enfermera enfermera;
    Enfermera enf;

    fread(&enfermera,sizeof(Enfermera),1,flujo);
	if(!feof(flujo))
 	{	
      
       *(p+contador)=enfermera;
        contador++;
       
 	 	return ExtraerEnfermeras(flujo,p ,contador);
 	}

 return ; 	
}

//FUNCIONES RESPIRADOR

int UsaRespirador(FILE *flujo,int idres,int band)
{
	Respirador respirador;
    fread(&respirador,sizeof(Respirador),1,flujo);
    if(!feof(flujo))
 	{
 		if(respirador.id_paciente==idres)
 		{
 			band=1;
 		}
 	 	return UsaRespirador(flujo,idres,band);
 	}
 return band;
}

void CambiarRespiradordePaciente(FILE *flujo_respirador,FILE *flujo2_respirador,int idviejo,int idnuevo)
{
    Respirador respirador;
    fread(&respirador,sizeof(Respirador),1,flujo_respirador);
	if(!feof(flujo_respirador))
 	{	
 		
 		if(respirador.id_paciente==idviejo)
 		{
 			
 			respirador.id_paciente=idnuevo;
 			printf("ahora el respirador es de %i \n",respirador.id_paciente );
 		}
 		fwrite(&respirador,sizeof(Respirador),1,flujo2_respirador); 	
 	 	return CambiarRespiradordePaciente(flujo_respirador,flujo2_respirador,idviejo ,idnuevo);
 	}
 return;
}

int ContarRespiradores(FILE *flujo,int contador)
{
 Respirador respirador;
 fread(&respirador,sizeof(Respirador),1,flujo);
	if(!feof(flujo))
 	{	
 		contador++;
 	 	return ContarRespiradores(flujo,contador);
 	}
 
 return contador; 	
}


int ContarEnfermeras(FILE *flujo,int contador)
{
 Enfermera enfermera;
 fread(&enfermera,sizeof(Enfermera),1,flujo);
	if(!feof(flujo))
 	{	
 		contador++;
 	 	return ContarEnfermeras(flujo,contador);
 	}
 
 return contador; 	
}

int ContarCamasTotales(FILE *flujo,int contador)
{
  Cama cama; 
  fread(&cama,sizeof(Cama),1,flujo);	
	if(!feof(flujo))
 	{	
 	 contador++;
 	 return ContarCamasTotales(flujo,contador);
 	}
 return contador; 	
}


int ContarCamasLibres(FILE *flujo,int contador)
{
  Cama cama;
  fread(&cama,sizeof(Cama),1,flujo);
	if(!feof(flujo))
 	{	
 		if(cama.id_enfermera==0)
 		{
 		contador++;
 	 	}
 	 return ContarCamasLibres(flujo,contador);
 	}
 return contador; 	
}
int ContarPacientesA(FILE *flujo,int contador)
{
 Paciente paciente;
 fread(&paciente,sizeof(Paciente),1,flujo);
	if(!feof(flujo))
 	{	
 		if(paciente.estado=='A')
 		{
 		 contador++;
 	 	}
 	 	return ContarPacientesA(flujo,contador);
 	}
 
 return contador; 	
}

int ContarPacientesB(FILE *flujo,int contador)
{
 Paciente paciente;
 fread(&paciente,sizeof(Paciente),1,flujo);
	if(!feof(flujo))
 	{	
 		if(paciente.estado=='B')
 		{
 		 contador++;
 	 	}
 	 	return ContarPacientesB(flujo,contador);
 	}
 
 return contador; 	
}

int AsignarRespirador(FILE *flujo1,FILE *flujo2,int id_paciente,int band)
{
	Respirador respirador;
    fread(&respirador,sizeof(Respirador),1,flujo1);
	if(!feof(flujo1))
 	{
 		if(respirador.id_paciente==0 && band==0)
 		{
 			respirador.id_paciente=id_paciente;
 			band=1;
 		}
 	 	fwrite(&respirador,sizeof(Respirador),1,flujo2);
 	 	return AsignarRespirador(flujo1,flujo2,id_paciente,band);
 	}
 return band; 	
}

int ValidarIDRespirador(FILE *flujo,int id){
	Respirador respirador;
	if(!feof(flujo))
 	{
 	  fread(&respirador,sizeof(Respirador),1,flujo);
 		if(respirador.id_respirador==id)
 		{
 			return 0;
 		}	

 		if(respirador.id_respirador!=id)
 		{
 			return ValidarIDRespirador(flujo,id);
 		}
 	}
 	return 1 ;
}


void EliminarRespirador(FILE *flujo,FILE *flujo2,int idres)
{
	Respirador respirador;
    fread(&respirador,sizeof(Respirador),1,flujo);
	if(!feof(flujo))
 	{
 		if(idres!=respirador.id_respirador)
 		{
 		fwrite(&respirador,sizeof(Respirador),1,flujo2);
 		}		   
 		return EliminarRespirador(flujo,flujo2,idres);
 	}
 return ;
}


int ValidarIDCama(FILE *flujo,int id){
	Cama cama;
 	fread(&cama,sizeof(Cama),1,flujo);	
	if(!feof(flujo))
 	{
 		if(cama.id_cama==id)
 		{
 			return 0;
 		}	
 		if(cama.id_cama!=id)
 		{
 			return ValidarIDCama(flujo,id);
 		}
 	}
 return 1 ;
}

int RetirarRespirador(FILE *flujo,FILE *flujo2,int idres,int band)
{
	Respirador respirador;
    fread(&respirador,sizeof(Respirador),1,flujo);
	if(!feof(flujo))
 	{
 		if(idres==respirador.id_respirador)
 		{
 			band=1;
 			respirador.id_paciente=0;
 		}
 		fwrite(&respirador,sizeof(Respirador),1,flujo2);
 		return RetirarRespirador(flujo,flujo2,idres,band);
 	}
 return band;
}

void Alta_Respirador()
{
	Respirador nuevo;
	FILE  *flujo;
	int id;
	int contador=0;
	printf("Ingresa el Modelo \n");
	scanf("%s",nuevo.modelo);
    flujo=fopen("Respiradores.bin","ab");	
	fclose(flujo);
	flujo=fopen("Respiradores.bin","rb");
	while(contador!=1)
	{
	 id = GeneraID();
	 contador=ValidarIDRespirador(flujo,id);		
 	}
 	fclose(flujo);
	nuevo.id_respirador=id;
	nuevo.id_paciente=0;
    
	flujo=fopen("Respiradores.bin","ab");	
	fwrite(&nuevo,sizeof(Respirador),1,flujo);
	fclose(flujo);
	return;	
}

void Alta_Cama()
{
	Cama nuevo;
	FILE  *flujo;
	int id;
	int contador=0;

	while(contador!=1)
	{
      flujo=fopen("Camas.bin","ab");	
	fclose(flujo);
	 flujo=fopen("Camas.bin","rb");		
	 id = GeneraID();
	 contador=ValidarIDCama(flujo,id);		
     fclose(flujo);
 	}
	nuevo.id_cama=id;
	nuevo.id_paciente=0;
	nuevo.id_enfermera=0;
    
	flujo=fopen("Camas.bin","ab");	
	fwrite(&nuevo,sizeof(Cama),1,flujo);
	fclose(flujo);
	printf("Cama dada de alta \n"); 
    return;
}

void MostrarRespiradores(FILE *flujo)
{
 Respirador respirador;
 fread(&respirador,sizeof(Respirador),1,flujo);
	if(!feof(flujo))
 	{
 	 	printf(" %i ",respirador.id_respirador);
 	 	printf(" %s ",respirador.modelo);
 	 	if(respirador.id_paciente==0)
 	 	{
 	 		printf("LIBRE \n");
 	 	}
 	 	else
 	 	{
 	 		printf("OCUPADO \n");
 	 	}	
 	 	return MostrarRespiradores(flujo);
 	}
 
 return ; 	
}

int QuitarRespirador(FILE *flujo,FILE *flujo2,int id,int band)
{
	Respirador respirador;
	fread(&respirador,sizeof(Respirador),1,flujo);
	if(!feof(flujo))
 	{
 		if(respirador.id_paciente==id)
 		{
 			respirador.id_paciente=0;
 			band=1;
 		}	

		 fwrite(&respirador,sizeof(Respirador),1,flujo2);
		 return QuitarRespirador(flujo,flujo2,id,band); 		
 	}
   fclose(flujo);
   fclose(flujo2);
   return band ;
}

int QuitarPacienteDeCama(FILE *flujo,FILE *flujo2,int id_paciente,int band)
{
	Cama cama; 
	fread(&cama,sizeof(Cama),1,flujo);
	if(!feof(flujo))
 	{
 		if(cama.id_paciente==id_paciente && band!=1)
 		{
 			cama.id_paciente=0;
 			band=1;
 		}	
   	  fwrite(&cama,sizeof(Cama),1,flujo2);
   	  return QuitarPacienteDeCama(flujo,flujo2,id_paciente,band);		
 	}
 	return  band;
}


int EliminarPaciente(FILE *flujo_Pacientes,FILE *flujo_Pacientes2,int idpaciente,int band)
 {

 	Paciente paciente;
 	int x;
 	int y;
 	int z;
 	FILE *flujo_respirador;
 	FILE *flujo2_respirador;
 	FILE *flujo_cama;
 	FILE *flujo2_cama;
 	int contadorres;
 	Paciente *p;
 	fread(&paciente,sizeof(Paciente),1,flujo_Pacientes);
	if(!feof(flujo_Pacientes))
 	{
 		if(paciente.id_paciente!=idpaciente)
 		{
 		   fwrite(&paciente,sizeof(Paciente),1,flujo_Pacientes2);     
 		}
 		if(paciente.id_paciente==idpaciente && paciente.estado!='B')
 		{
 			paciente.estado='B';
 			fwrite(&paciente,sizeof(Paciente),1,flujo_Pacientes2); 
 		    flujo_respirador=fopen("Respiradores.bin","rb");
 			flujo2_respirador=fopen("Respiradores2.bin","ab");
 		  	y=QuitarRespirador(flujo_respirador,flujo2_respirador,paciente.id_paciente,0);  	  	
 		  	fclose(flujo_respirador);
 			fclose(flujo2_respirador);
 		  	if(y==1)
 		  	{
 		  	  printf("Respirador Quitado del Paciente\n");
 		 	  remove("Respiradores.bin");
	     	  rename("Respiradores2.bin","Respiradores.bin");
 		  	}
 		  	else
 		  	{
 		  	  printf("EL paciente no tiene Respirador\n");
 		  	  remove("Respiradores2.bin");
 		  	}
 		  	flujo_cama=fopen("Camas.bin","rb");
 			flujo2_cama=fopen("Camas2.bin","ab");
 		  	z=QuitarPacienteDeCama(flujo_cama,flujo2_cama,paciente.id_paciente,0);  	  	
 		  	fclose(flujo_cama);
 			fclose(flujo2_cama);
 			if(z==1)
 		  	{
 		  	  printf("Paciente Retirado de Cama\n");
 		 	  remove("Camas.bin");
	     	  rename("Camas2.bin","Camas.bin");
 		  	}
 		  	else
 		  	{
 		  	  printf("EL paciente no se encontro en alguna cama\n");
 		  	  remove("Camas2.bin");
 		  	}
 		  	band=1;
 		}
 	 	return EliminarPaciente(flujo_Pacientes,flujo_Pacientes2,idpaciente,band);
 	}
 return band ; 	
}

Enfermera Cama_Enfermera(FILE *flujo, int idenfermera,Enfermera enf)
{
 Enfermera enfermera;
 int x;
 fread(&enfermera,sizeof(Enfermera),1,flujo);
	if(!feof(flujo))
 	{
 		if(enfermera.id_enfermera==idenfermera)
 		{

 			enf=enfermera;
 			return enf;
 		}
	}
 return Cama_Enfermera(flujo,idenfermera,enf);
}

void MostrarCamasLibres(FILE *flujo)
{
 Cama cama;
 Enfermera enfermera;
 FILE *flujo_enfermera;
 Enfermera enf;
 fread(&cama,sizeof(Cama),1,flujo);
	
	if(!feof(flujo))
 	{
	
		if(cama.id_paciente==0 && cama.id_enfermera!=0)
	 	 {
	 	 	flujo_enfermera=fopen("Enfermeras.bin","rb");
	 	 	enfermera=Cama_Enfermera(flujo_enfermera,cama.id_enfermera,enf);
	 	 	printf(" %i  ",cama.id_cama );
	 	 	printf(" %s  ",enfermera.nombre );
	 	 	printf(" %s \n",enfermera.app );
	 	 	fclose(flujo_enfermera);
	 	 }
 	 	return MostrarCamasLibres(flujo);
 	}
		
 return ; 	
}
int DarCamaPaciente(FILE *flujo,FILE *flujo2,int id_paciente,int id_cama,int band)
{
	Cama cama;
	 
	fread(&cama,sizeof(Cama),1,flujo);
	if(!feof(flujo))
 	{
 	 
 		if(id_cama==cama.id_cama && band!=1 && cama.id_enfermera!=0)
 		{

 			cama.id_paciente=id_paciente;
 			band=1;
 		}	
   	  fwrite(&cama,sizeof(Cama),1,flujo2);
   	  return DarCamaPaciente(flujo,flujo2,id_paciente,id_cama,band); 		
 	}
 	return  band;
}

int  VerificarCamaLibre(FILE *flujo,int id_cama,int band)
{
 Cama cama;
 fread(&cama,sizeof(Cama),1,flujo);
	
	if(!feof(flujo))
 	{
	
		if(cama.id_cama==id_cama && cama.id_enfermera!=0  && cama.id_paciente==0)
	 	 {
	 	 
			band=1;	 	 	
	 	 }
 	 	return VerificarCamaLibre(flujo,id_cama,band);
 	}
		
 return band; 	
}


int ModificarPaciente(FILE *flujo_Pacientes,FILE *flujo_Pacientes2,int idpaciente,int band)
{
 	Paciente paciente;
 	int x;
 	int y;
 	int z;
 	int x1;
 	char opc='S';
 	FILE *flujo_respirador;
 	FILE *flujo2_respirador;
 	FILE *flujo_cama;
 	FILE *flujo2_cama;
 	int contadorres;
 	Paciente *p;
 	fread(&paciente,sizeof(Paciente),1,flujo_Pacientes);
	if(!feof(flujo_Pacientes))
 	{
 		if(paciente.id_paciente==idpaciente && paciente.estado=='A')
 		{
 			do
 			{
 				printf("PACIENTE ENCONTRADO\n");
 				printf("1.-Modificar Nombre\n");
 				printf("2.-Modificar APP\n");
 		    	printf("3.-Modificar APM\n");
 		    	printf("4.-Quitar Respirador\n");
 		    	printf("5.-Agregar Respirador\n");
 		    	printf("6.-Mover de Cama\n");
 		    	scanf("%i",&x);
 		    	switch(x)
 		    	{
 		    		case 1:
 		    			printf("Ingresa el nuevo Nombre\n");
 		    			scanf("%s",paciente.nombre);
 		    			band=1;	
 		    		break;
 		    		case 2:
 		    			printf("Ingresa el Apellido Paterno \n");
 		    			scanf("%s",paciente.app);
 		    			band=1;
 		    		break;
 		    		case 3:
 		    			printf("Ingresa el Apellido Materno\n");
 		    			scanf("%s",paciente.apm);
 		    			band=1;
 		    		break;
 		    		case 4:
 		    	 		flujo_respirador=fopen("Respiradores.bin","rb");
 			     		flujo2_respirador=fopen("Respiradores2.bin","ab");
 		  	  	 		y=QuitarRespirador(flujo_respirador,flujo2_respirador,paciente.id_paciente,0);  	  	
 		  	     		fclose(flujo_respirador);
 				 		fclose(flujo2_respirador);
 		  	  			if(y==1)
 		  	  			{
 		  	  		 		printf("Respirador Quitado del Paciente\n");
 		 			 		remove("Respiradores.bin");
	     			 		rename("Respiradores2.bin","Respiradores.bin");
 		  	  				band=1;
 		  	  			}
 		  	  			else
 		  	  			{
 		  	  		 		printf("EL paciente no tiene Respirador\n");
 		  	  		 		remove("Respiradores2.bin");
 		  	  			}
 		    		break;
 		    		case 5:
 		    			flujo_respirador=fopen("Respiradores.bin","rb");
 		    	 		y=UsaRespirador(flujo_respirador,paciente.id_paciente,0);
 		    	 		fclose(flujo_respirador);
 		    	 		if(y==0)
 		    	 		{	
		 				 flujo_respirador=fopen("Respiradores.bin","rb");
 						 flujo2_respirador=fopen("Respiradores2.bin","ab");
	 					 y=AsignarRespirador(flujo_respirador,flujo2_respirador,paciente.id_paciente,0);
	 			 		 fclose(flujo_respirador);
     			 		 fclose(flujo2_respirador);
     			 		 remove("Respiradores.bin");
	 			 	 	 rename("Respiradores2.bin","Respiradores.bin");
    						if(y==0)
    						{
    	 			 		 printf("Todos los Respiradores estan Ocupados \n");
    	 			 		 flujo_respirador=fopen("Respiradores.bin","rb");
    	 			 		 contadorres=ContarRespiradores(flujo_respirador,0);
    	 			 		 fclose(flujo_respirador);
         			 		 p = (Paciente*)malloc ( (contadorres)*sizeof(Paciente) );
    	 			 		 flujo_respirador=fopen("Respiradores.bin","rb");
    	 			 		 PacientesConRespiradores(flujo_respirador,p,0);
    	 			 		 fclose(flujo_respirador);
    	 			 		 OrdenarResPac(p,contadorres);
	    						if(paciente.edad<(p->edad))
    							{
    	   	 			 		 flujo_respirador=fopen("Respiradores.bin","rb");
 			 			 		 flujo2_respirador=fopen("Respiradores2.bin","ab");
 			 			 		 CambiarRespiradordePaciente(flujo_respirador,flujo2_respirador,p->id_paciente,paciente.id_paciente);
    					  		 printf("se le quito el respirador al paciente mas viejo\n");
    		 			 		 fclose(flujo_respirador);
    		 			 		 fclose(flujo2_respirador);
   			 			 		 remove("Respiradores.bin");
	         			 		 rename("Respiradores2.bin","Respiradores.bin");
	    	    	    		 printf("Respirador Asignado\n");
    	    					 band=1;
    	    					}
    							else
    	 						{
    		   			 		 printf("No existen Pacientes mayores al dado de alta \n");
    							} 
	    			        }
	    		        }
	    				else
	    				{
	    				 printf("El paciente ya tiene  respirador \n");
	    				}  
			        break;
			        case 6:
			    	 flujo_cama=fopen("Camas.bin","rb");
			     	 printf("CAMA---ENFERMERA\n");
			     	 MostrarCamasLibres(flujo_cama);
			     	 fclose(flujo_cama);
			     	 printf("Selecciona la cama a donde quieres Mover al Paciente\n");
 		    	 	 scanf("%i",&z);
 		    	 	 flujo_cama=fopen("Camas.bin","rb");
 		    	 	 x1=VerificarCamaLibre(flujo_cama,z,0);
 				 	 fclose(flujo_cama);
 		    	 	 	if(x1==1)
 		    	 	    {
 		    	 	  	 printf("Procesando...\n");
 		    	 		 flujo_cama=fopen("Camas.bin","rb");
 		    	 	 	 flujo2_cama=fopen("Camas2.bin","ab");
 		    	 	 	 y=QuitarPacienteDeCama(flujo_cama,flujo2_cama,paciente.id_paciente,0);
 		    	 		 fclose(flujo_cama);
    		 	 		 fclose(flujo2_cama);
   			 	 		 remove("Camas.bin");
	         	 		 rename("Camas2.bin","Camas.bin");
	         	 		 flujo_cama=fopen("Camas.bin","rb");
 		    	 		 flujo2_cama=fopen("Camas2.bin","ab");
	         	 		 x=DarCamaPaciente(flujo_cama,flujo2_cama,paciente.id_paciente,z,band);
	         	 		 fclose(flujo_cama);
    		 	 		 fclose(flujo2_cama);
	         	 			if(x==1)
	         	 			{
	         	 			 printf("Paciente Trasladado\n");
	         	 			 remove("Camas.bin");
	         	    		 rename("Camas2.bin","Camas.bin");
	         	 			 band=1;
	         	 			}
	         	 			else
	         	 			{
	         	 			 remove("Camas2.bin");
	         	 			 printf("error\n");
 		    	 			}
 		    			}
 		    			else
 		    			{
 		    			 printf("ERROR CAMA NO DISPONIBLE\n");
 		    			}
 		    		break;
 		    	}
 		 	 printf("Preciona la tecla s para seguir modificando // Preciona la tecla n para terminar la modificacion\n");
 		 	 scanf(" %c",&opc);   
 			}while(opc=='S' || opc=='s');	     
 		}
	 fwrite(&paciente,sizeof(Paciente),1,flujo_Pacientes2);	 
 	 return ModificarPaciente(flujo_Pacientes,flujo_Pacientes2,idpaciente,band);
 	}
 return band; 	
}

int AsignarCama(FILE *flujo1,FILE *flujo2,int id_paciente,int band)
{
	Cama cama;
    fread(&cama,sizeof(Cama),1,flujo1);
	if(!feof(flujo1))
 	{
 		if(cama.id_paciente==0 && cama.id_enfermera!=0 && band==0)
 		{
 			 cama.id_paciente=id_paciente;
 			band=1;
 		}
 	 	fwrite(&cama,sizeof(Cama),1,flujo2);
 	 	return AsignarCama(flujo1,flujo2,id_paciente,band);
 	}
 return band; 	
}


int AsignarCamaEnfermera(FILE *flujo1,FILE *flujo2,int id_enfermera,int band)
{
	Cama cama;
    fread(&cama,sizeof(Cama),1,flujo1);
	if(!feof(flujo1))
 	{
 		if(cama.id_enfermera==0 &&  band==0)
 		{
 			 cama.id_enfermera=id_enfermera;
 			band=1;
 		}
 	 	fwrite(&cama,sizeof(Cama),1,flujo2);
 	 	return AsignarCamaEnfermera(flujo1,flujo2,id_enfermera,band);
 	}
 return band; 	
}

int ValidarIDPaciente(FILE *flujo,int id){
	Paciente paciente;
	
	if(!feof(flujo))
 	{
 	  fread(&paciente,sizeof(Paciente),1,flujo);
 		if(paciente.id_paciente==id)
 		{
 			return 0;
 		}	

 		if(paciente.id_paciente!=id)
 		{
 			return ValidarIDPaciente(flujo,id);
 		}
 	}
 	return 1 ;
}

void Alta_Paciente()
{
	Paciente nuevo;
	FILE *flujo;
	FILE *flujo_respirador;
	FILE *flujo2_respirador;
	FILE *flujo_cama;
	FILE *flujo2_cama;
	int band2;
	Paciente *p;
	int contadorres;
	int id;
	int contador=0;
	int opcion;
	char res;
	int band;
	int idcambiar;
	printf("Ingresa el nombre \n");
	scanf("%s",nuevo.nombre);
	printf("Ingresa el apellido paterno \n");
	scanf("%s",nuevo.app);
	printf("Ingresa el apellido materno\n");
	scanf("%s",nuevo.apm);
 	do
 	{
     printf(" Genero  \n");
 	 printf("1.Masculino\n");
 	 printf("2.Femenino\n");
 	 scanf(" %i",&opcion);
 		switch(opcion)
 		{
 		 case 1:
 		 nuevo.genero='M';	
 		 break;
 		 case 2:
 		 nuevo.genero='F';
 		 break;
 		}
    }while(opcion>2);
	printf("Ingresa la Edad \n");
	scanf("%i",&nuevo.edad);
    flujo=fopen("Pacientes.bin","ab");	
    fclose(flujo);
	flujo=fopen("Pacientes.bin","rb");		
		while(contador!=1)
		{
	 	 id = GeneraID();
	 	 contador=ValidarIDPaciente(flujo,id);
		}
	fclose(flujo);
	nuevo.id_paciente=id;
 	flujo_cama=fopen("Camas.bin","rb");
 	flujo2_cama=fopen("Camas2.bin","ab");
	band2=AsignarCama(flujo_cama,flujo2_cama,nuevo.id_paciente,0);
	fclose(flujo_cama);
    fclose(flujo2_cama);
    remove("Camas.bin");
	rename("Camas2.bin","Camas.bin");
	if(band2==1)
	{
	 printf("Cama asignada y paciente dado de alta \n");
	 flujo=fopen("Pacientes.bin","ab");
	 nuevo.estado='A';
	 fwrite(&nuevo,sizeof(Paciente),1,flujo);
	 fclose(flujo);
	 printf("Paciente Necesita Respirador? S/N \n");
	 scanf(" %c",&res);
	 if(res=='S' || res=='s')
		{
	 	 flujo_respirador=fopen("Respiradores.bin","rb");
 	 	 flujo2_respirador=fopen("Respiradores2.bin","ab");
	 	 band=AsignarRespirador(flujo_respirador,flujo2_respirador,nuevo.id_paciente,0);
	 	 fclose(flujo_respirador);
     	 fclose(flujo2_respirador);
     	 remove("Respiradores.bin");
	 	 rename("Respiradores2.bin","Respiradores.bin");
    		if(band==0)
    		{
    	 	 printf("Todos los Respiradores estan Ocupados \n");
    	 	 flujo_respirador=fopen("Respiradores.bin","rb");
    	 	 contadorres=ContarRespiradores(flujo_respirador,0);
    	 	 fclose(flujo_respirador);
         	 p = (Paciente*)malloc ( (contadorres)*sizeof(Paciente) );
    	 	 flujo_respirador=fopen("Respiradores.bin","rb");
    	 	 PacientesConRespiradores(flujo_respirador,p,0);
    	 	 fclose(flujo_respirador);
    	     OrdenarResPac(p,contadorres);
	    	 	if(nuevo.edad<(p->edad))
    			{
    	   	 	 flujo_respirador=fopen("Respiradores.bin","rb");
 			 	 flujo2_respirador=fopen("Respiradores2.bin","ab");
 			 	 CambiarRespiradordePaciente(flujo_respirador,flujo2_respirador,p->id_paciente,nuevo.id_paciente);
    		 	 printf("se le quito el respirador al paciente mas viejo\n");
    		 	 fclose(flujo_respirador);
    		 	 fclose(flujo2_respirador);
   			 	 remove("Respiradores.bin");
	         	 rename("Respiradores2.bin","Respiradores.bin");
  			 	 printf("Respirador Asignado\n");
    	    	}
    			else
    	 		{
    			 printf("No existen Pacientes mayores al dado de alta \n");
    			}
	        }
	    }
    }
    else
	{
	 printf("No existen camas disponibles busca ayuda en otro Hospital\n");
	}
 return;
}

void Cama_Paciente(FILE *flujo,int id_paciente)
{
 Cama cama;
 fread(&cama,sizeof(Cama),1,flujo);
	
	if(!feof(flujo))
 	{
	
		if(id_paciente==cama.id_paciente)
	 	{
	 	 	printf("%i |\n",cama.id_cama );
	 	}
 	 	return Cama_Paciente(flujo,id_paciente);
 	}
 return ; 	
}

Paciente CamaPaciente(FILE *flujo, int idpaciente,Paciente pac)
{
 Paciente paciente;
 int x;
 fread(&paciente,sizeof(Paciente),1,flujo);
	if(!feof(flujo))
 	{
 		if(paciente.id_paciente==idpaciente)
 		{

 			pac=paciente;
 			return pac;
 		}
	}
 return CamaPaciente(flujo,idpaciente,pac);
}


void MostrarCamas(FILE *flujo)
{
 Cama cama;
 Enfermera aux;
 Enfermera enfermera;
 Paciente paciente;
 Paciente aux2;
 int x;
 FILE *flujo_enfermera;
 FILE *flujo_paciente;
 fread(&cama,sizeof(Cama),1,flujo);
	
	if(!feof(flujo))
 	{

	 	if(cama.id_enfermera!=0)
	    {

 		flujo_enfermera=fopen("Enfermeras.bin","rb");
	 	enfermera=Cama_Enfermera(flujo_enfermera,cama.id_enfermera,aux);
	 	fclose(flujo_enfermera);
	 	printf(" %i ",cama.id_cama);
	    printf("| %s ",enfermera.nombre );
	    printf(" %s ",enfermera.app );
	    printf(" %s |",enfermera.apm );
	    }
	    if(cama.id_enfermera==0)
		{
	    printf(" %i ",cama.id_cama);	
		printf("SINENFERMERA ");
		}
		if(cama.id_paciente==0)
		{
			printf("SinPaciente\n");	
		}
		if(cama.id_paciente!=0)
		{
		 flujo_paciente=fopen("Pacientes.bin","rb");
	 	 paciente=CamaPaciente(flujo_paciente,cama.id_paciente,aux2);
	 	 printf("|%s ",paciente.nombre );
	 	 printf("%s ",paciente.app );
	 	 printf("%s|\n",paciente.apm );
	 	 fclose(flujo_paciente);
		}	

 	 	return MostrarCamas(flujo);
 	}
 return ; 	
}



void MostrarPacientes(FILE *flujo)
{
 Paciente paciente;
 int x;
 FILE *flujo_respirador;
 FILE *flujo_cama;
 fread(&paciente,sizeof(Paciente),1,flujo);
	if(!feof(flujo))
 	{

	    if(paciente.estado=='A')
 		{
 		 flujo_respirador=fopen("Respiradores.bin","rb");
 	 	 printf("| %i ",paciente.id_paciente);
 	 	 printf(" %s ",paciente.nombre);
 	 	 printf(" %s ",paciente.app);
 	 	 printf(" %s ",paciente.apm);
 	 	 printf(" %c ",paciente.genero );
	 	 printf(" %i ",paciente.edad );
	 	 x=UsaRespirador(flujo_respirador,paciente.id_paciente,0);
	 		if(x==1)
	 	 	{
	 		 printf("CON RESPIRADOR ");
	 		}
	 		if(x!=1)
	 		{
	 		printf("SIN RESPIRADOR ");
	 		}
	 	 fclose(flujo_respirador);
 		}
 		flujo_cama=fopen("Camas.bin","rb");
 		Cama_Paciente(flujo_cama,paciente.id_paciente);
 		fclose(flujo_cama);
 		return MostrarPacientes(flujo);
 	}
 return ; 	
}

void MostrarPacientesEliminados(FILE *flujo)
{
 Paciente paciente;
 int x;
 FILE *flujo_respirador;
 fread(&paciente,sizeof(Paciente),1,flujo);
	if(!feof(flujo))
 	{
 		if(paciente.estado=='B')
 		{
 	 	 printf(" %i ",paciente.id_paciente);
 	 	 printf(" %s ",paciente.nombre);
 	 	 printf(" %s ",paciente.app);
 	 	 printf(" %s ",paciente.apm);
 	 	 printf(" %c ",paciente.genero );
	 	 printf(" %i ",paciente.edad );
	 	 printf("\n");
 	 	
 		}
 		 return MostrarPacientesEliminados(flujo);
    }

 return ; 	
}
int QuitarCama(FILE *flujo,FILE *flujo2,int id_enfermera,int id_cama,int band)
{
	Cama cama; 
	fread(&cama,sizeof(Cama),1,flujo);
	if(!feof(flujo))
 	{
 	 
 		if(cama.id_cama==id_cama && cama.id_enfermera==id_enfermera && band!=1)
 		{
 			cama.id_enfermera=0;
 			band=1;
 		}	
   	    fwrite(&cama,sizeof(Cama),1,flujo2);
   	    return QuitarCama(flujo,flujo2,id_enfermera,id_cama,band);
 	}
 	return  band;
}

int AgregarCamaEnfermera(FILE *flujo,FILE *flujo2,int id_enfermera,int id_cama,int band){
	Cama cama;
	 
	fread(&cama,sizeof(Cama),1,flujo);
	if(!feof(flujo))
 	{
 	 
 		if(cama.id_cama==id_cama && cama.id_enfermera==0 && band!=1)
 		{
 			cama.id_enfermera=id_enfermera;
 			band=1;
 		}	

   	  fwrite(&cama,sizeof(Cama),1,flujo2);
   	  return AgregarCamaEnfermera(flujo,flujo2,id_enfermera,id_cama,band);	
 	}
 	return  band;
}

void  MostrarCamasDeEnfermera(FILE *flujo,int id){
	Cama cama;
	FILE *flujo_paciente;
	Paciente aux2;
	Paciente paciente;
	if(!feof(flujo))
 	{
 	  fread(&cama,sizeof(Cama),1,flujo);
 		if(cama.id_enfermera==id)
 		{
 		 printf("|%i ",cama.id_cama );
 		 if(cama.id_paciente!=0)
 		 {
		 flujo_paciente=fopen("Pacientes.bin","rb");
	 	 paciente=CamaPaciente(flujo_paciente,cama.id_paciente,aux2);
	 	 printf("|%s ",paciente.nombre );
	 	 printf("%s ",paciente.app );
	 	 printf("%s|\n",paciente.apm );
	 	 fclose(flujo_paciente);
 		 }
 		 else
 		 {
 			printf("Sin paciente \n");
 		 }
 		}	
 		 return MostrarCamasDeEnfermera(flujo,id);	
 	}
 	return  ;
}


void  MostrarCamasSinEnfermera(FILE *flujo){
	Cama cama;
	FILE *flujo_paciente;
	Paciente aux2;
	Paciente paciente;
	if(!feof(flujo))
 	{
 	  fread(&cama,sizeof(Cama),1,flujo);
 		if(cama.id_enfermera==0)
 		{
 		 printf("|%i ",cama.id_cama );
 		 if(cama.id_paciente!=0)
 		 {
		 flujo_paciente=fopen("Pacientes.bin","rb");
	 	 paciente=CamaPaciente(flujo_paciente,cama.id_paciente,aux2);
	 	 printf("|%s ",paciente.nombre );
	 	 printf("%s ",paciente.app );
	 	 printf("%s|\n",paciente.apm );
	 	 fclose(flujo_paciente);
 		}
 		else
 		{
 			printf("Sin paciente \n");
 		}

 		}	
 		return MostrarCamasSinEnfermera(flujo);	
 	}
 	return  ;
}


int ValidarIDEnfermera(FILE *flujo,int id){
	Enfermera enfermera;
	if(!feof(flujo))
 	{
 	    fread(&enfermera,sizeof(Enfermera),1,flujo);
 		if(enfermera.id_enfermera==id)
 		{
 			return 0;
 		}	

 		if(enfermera.id_enfermera!=id)
 		{
 			return ValidarIDEnfermera(flujo,id);
 		}
 	}
 	return 1 ;
}

void ModificarEnfermera(FILE *flujo_Enfermera,FILE *flujo_Enfermera2,int idenfermera)
{

 	Enfermera enfermera;
 	int x;
 	int y;
 	char opc='S';
 	FILE *flujo_cama;
 	FILE *flujo2_cama;
 	int ncama;
 	int contadorcam;
 	Enfermera *p;
 	fread(&enfermera,sizeof(Enfermera),1,flujo_Enfermera);
	if(!feof(flujo_Enfermera))
 	{
 		if(enfermera.id_enfermera==idenfermera)
 		{
 			do{


 			printf("Enfermera Encontrada\n");
 			printf("1.-Modificar Nombre\n");
 			printf("2.-Modificar APP\n");
 		    printf("3.-Modificar APM\n");
 		    printf("4.-Modificar Telefono\n");
 		    printf("5.-Modificar Direccion\n");
 		    printf("6.-Modificar Codigo Postal\n");
 		    printf("7.-Quitar Cama\n");
 		    printf("8.-Agregar Cama\n");
 		    scanf("%i",&x);
 		    switch(x)
 		    {
 		    	case 1:
 		    	printf("Ingresa el nuevo Nombre\n");
 		    	scanf("%s",enfermera.nombre);
 		    	break;
 		    	case 2:
 		    	printf("Ingresa el Apellido Paterno \n");
 		    	scanf("%s",enfermera.app);
 		    	break;
 		    	case 3:
 		    	printf("Ingresa el Apellido Materno\n");
 		    	scanf("%s",enfermera.apm);
 		    	break;
 		    	case 4:
 		    	printf("Ingresa el Telefono\n");
 		    	scanf("%s",enfermera.telefono);
 		    	break;
 		    	case 5:
 		    	printf("Ingresa la nueva Direccion \n");
 		    	 fflush( stdin );
				 scanf("%[^\n]",enfermera.direccion);
	 			 fflush( stdin );
 		    	break;
 		    	case 6:
 		    	printf("Ingresa el codigo postal nuevo\n");
 		    	scanf("%s",enfermera.codpostal);
 		    	break;
 		    	case 7:
 		    	 flujo_cama=fopen("Camas.bin","rb");
 			     printf("Camas De la Enfermera \n");
 			     printf("CAMA---NOM---APP--APM\n");
 			     MostrarCamasDeEnfermera(flujo_cama,enfermera.id_enfermera);
 			     fclose(flujo_cama);
 			     printf("Seleccion la Cama que quieres quitar \n");
 			     scanf("%i",&ncama);
 			     flujo_cama=fopen("Camas.bin","rb");
 			     flujo2_cama=fopen("Camas2.bin","ab");
 			     y=QuitarCama(flujo_cama,flujo2_cama,enfermera.id_enfermera,ncama,0);
 			     fclose(flujo_cama);
 			     fclose(flujo2_cama);
 			   
	     		 if(y!=0)
	     		 {
	     		 	printf("La enfermera fue retirada de la Cama\n");
	     			  remove("Camas.bin");
	     			 rename("Camas2.bin","Camas.bin");
	     		 }
	     		 else
	     		 {	
	     		 		  remove("Camas2.bin");
	     		 	printf("Asegurate de escribir bien el id correspondiente de la cama\n");
 		 		 }
 		    	 break;
 		    	 case 8:
 		    	 y=0;
 		    	 flujo_cama=fopen("Camas.bin","rb");
 		    	 y=contarCamasDeEnfermera(flujo_cama,enfermera.id_enfermera,0);
 		    	 fclose(flujo_cama);
 		    	 if(y<5)
 		    	 {
 		    	 flujo_cama=fopen("Camas.bin","rb");
 			     printf("Camas Sin Enfermera \n");
 			     printf("CAMA---NOM---APP--APM\n");
 			     MostrarCamasSinEnfermera(flujo_cama);
 			     fclose(flujo_cama);
 			     printf("Seleccion la Cama que quieres quitar \n");
 			     scanf("%i",&ncama);
 			     flujo_cama=fopen("Camas.bin","rb");
 			     flujo2_cama=fopen("Camas2.bin","ab");
 			     y=AgregarCamaEnfermera(flujo_cama,flujo2_cama,enfermera.id_enfermera,ncama,0);
 			     fclose(flujo_cama);
 			     fclose(flujo2_cama);
 			      if(y!=0)
	     		 {
	     		 	printf("La enfermera fue asignada a la cama\n");
	     			  remove("Camas.bin");
	     			 rename("Camas2.bin","Camas.bin");
	     		 }
	     		 else
	     		 {	
	     		 		  remove("Camas2.bin");
	     		 	printf("Asegurate de escribir bien el id correspondiente de la cama\n");
 		 		 }


 		    	 }
 		    	 else
 		    	 {
 		    	 	printf("Tu Enfermera ya tiene 5 camas asignadas \n");
 		    	 }

 		    	 break;

 		    	
 		    }
 		 printf("Preciona la tecla s para seguir modificando // Preciona la tecla n para terminar de modificar \n");
 		 scanf(" %c",&opc);   
 		}while(opc=='S' || opc=='s');	     
 		}
	     fwrite(&enfermera,sizeof(Enfermera),1,flujo_Enfermera2);
 		 
 	 	return ModificarEnfermera(flujo_Enfermera,flujo_Enfermera2,idenfermera);
 	}
 return ; 	


 }

void Alta_Enfermera()
{
	
	Enfermera nuevo;
	Cama cama;
	FILE  *flujo;
	FILE *flujo_Cama;
	FILE *flujo2_Cama;
	int c1;
	int ca;
	int id;
	int contador=0;
	int con;
	printf("Ingresa el nombre \n");
	scanf("%s",nuevo.nombre);
	printf("Ingresa el apellido paterno \n");
	scanf("%s",nuevo.app);
	printf("Ingresa el apellido materno\n");
	scanf("%s",nuevo.apm);
	printf("Ingresa el telefono\n");
	scanf("%s",nuevo.telefono);
	printf("Ingresa la direccion \n");
	 fflush( stdin );
	scanf("%[^\n]",nuevo.direccion);
	 fflush( stdin );
	printf("Ingresa la Edad\n");
	scanf("%i",&nuevo.edad);
	printf("Ingresa  codigo postal \n");
	scanf("%s",nuevo.codpostal);

    flujo=fopen("Enfermeras.bin","ab");	
    fclose(flujo);
		while(contador!=1)
	 	{
	     flujo=fopen("Enfermeras.bin","rb");
	 	 id = GeneraID();
	 	 contador=ValidarIDEnfermera(flujo,id);
		 fclose(flujo);		
		}
	nuevo.id_enfermera=id;
    
	flujo=fopen("Enfermeras.bin","ab");	
	fwrite(&nuevo,sizeof(Enfermera),1,flujo);
	fclose(flujo);
	flujo=fopen("Camas.bin","rb");
	c1=ContarCamasLibres(flujo,0);
	printf(" Numero de Camas Libres %i \n",c1);
	fclose(flujo);
	if(c1>0)
	{
 	printf("Ingresa el numero de camas que quieres asignar \n");
 	scanf("%i",&ca);
	con=0;
	 //GenerarCamas
	if(ca<6 && ca<=c1)
	{
	while(con<ca)
	{
	 flujo_Cama=fopen("Camas.bin","rb");	
	 flujo2_Cama=fopen("Camas2.bin","ab");		
	 AsignarCamaEnfermera(flujo_Cama,flujo2_Cama,nuevo.id_enfermera,0);
	 fclose(flujo_Cama);
	 fclose(flujo2_Cama);
	 remove("Camas.bin");
	 rename("Camas2.bin","Camas.bin");
	 con++;
	 printf("Asignando \n");
 	}
 	 printf("Camas Asignadas \n");
	}
	else
	{
		printf("error con las camas \n");
	}
	}
	else
	{
		printf("Enfermera dada de alta pero sin camas \n");
	}


	return;
}

void MostrarEnfermeras(FILE *flujo)
{
 Enfermera enfermera;
 fread(&enfermera,sizeof(Enfermera),1,flujo);
	if(!feof(flujo))
 	{
 	 	printf(" %i ",enfermera.id_enfermera);
 	 	printf(" %s ",enfermera.nombre);
 	 	printf(" %s ",enfermera.app);
 	 	printf(" %s ",enfermera.apm);
 	 	printf(" %i ",enfermera.edad);
 	 	printf(" %s ",enfermera.telefono);
 	 	printf(" %s \n",enfermera.direccion);
 	 	return MostrarEnfermeras(flujo);
 	}
 return ; 	
}

int QuitarEnfermeraDeCamas(FILE *flujo,FILE *flujo2,int id_enfermera,int band)
{
	Cama cama;
    fread(&cama,sizeof(Cama),1,flujo);
	if(!feof(flujo))
 	{
 		if(cama.id_enfermera==id_enfermera)
 		{

 			 cama.id_enfermera=0;
 			 band=1;
 		}
 	 	fwrite(&cama,sizeof(Cama),1,flujo2);
 	 	return QuitarEnfermeraDeCamas(flujo,flujo2,id_enfermera,band);
 	}
 return band; 	

}


void EliminarEnfermera(FILE *flujo,FILE *flujo2,int idenfermera)
 {
 	Enfermera enfermera;
 	int x;
 	int y;
 	FILE *flujo_cama;
 	FILE *flujo2_cama;
 	int contadorres;
 	Enfermera *p;
 	fread(&enfermera,sizeof(Enfermera),1,flujo);
	if(!feof(flujo))
 	{
 		if(enfermera.id_enfermera!=idenfermera)
 		{
 		   fwrite(&enfermera,sizeof(Enfermera),1,flujo2);     
 		}
 		if(enfermera.id_enfermera==idenfermera)
 		{
 		
 		    flujo_cama=fopen("Camas.bin","rb");
 			flujo2_cama=fopen("Camas2.bin","ab");
 		  	y=QuitarEnfermeraDeCamas(flujo_cama,flujo2_cama,enfermera.id_enfermera,0);  	  	
 		  	fclose(flujo_cama);
 			fclose(flujo2_cama);
 		  	if(y==1)
 		  	{
 		  	  printf("Enfermera Quitadada de las Camas\n");
 		 	  remove("Camas.bin");
	     	  rename("Camas2.bin","Camas.bin");
 		  	}
 		  	else
 		  	{
 		  	  printf("La enfermera no tenia Ninguna Cama asignada\n");
 		  	  remove("Camas2.bin");
 				
 		  	}
 		 }
 	 	return EliminarEnfermera(flujo,flujo2,idenfermera);
 	}
 return ; 	
}

int main()
{
 int cmenu=0;
 char lmenu='S';
 int cmenu2=0;
 int cmenu3=0;
 int x;
 int idres;
 int idpaciente;
 int idenfermera;
 int c1;
 int c2;
 Enfermera *p;
 FILE *flujo;
 FILE *flujo_Pacientes;
 FILE *flujo_Pacientes2;
 FILE *flujo_Enfermeras;
 FILE *flujo_Enfermeras2;
 FILE *flujo_respirador;
 FILE *flujo2_respirador;
 FILE *flujo_Cama;
 int contador;
	while( lmenu!='n')
	{	
	 BorrarPantalla();
 	 printf("***MENU*** \n");
	 printf("1.-Dar de Alta Enfermera\n");
 	 printf("2.-Lista Enfermeras\n");
 	 printf("3.-Dar de Alta Paciente \n");
 	 printf("4.-Lista Pacientes Dados de Alta \n");
 	 printf("5.-Agregar Nuevo Respirador \n");
 	 printf("6.-Lista de Respiradores \n");
 	 printf("7.-Lista Todas las  Enfermeras/Camas\n");
 	 printf("8.-Lista Pacientes Dados de Baja\n");
 	 printf("9.-Conteo de Pacientes/Camas/Enfermeras/Respiradores\n");
 	 printf("10.-Listado de Enfermeras con Camas Libres Ordenadas \n");
 	 printf("11.-Dar de Alta Cama \n");
 	 printf("12.-Salir \n");
 	 scanf("%i",&cmenu);
 	 BorrarPantalla();
		switch(cmenu)
		{
			case 1:
		 	Alta_Enfermera();
			break;
			case 2:
		 	    flujo_Enfermeras=fopen("Enfermeras.bin","rb");
				if(flujo_Enfermeras==NULL)
				{
			 	 printf("Error No se encontro el archivo bin de enfermeras\n");
				}
				else
				{
			    printf("***LISTA DE PERSONAL***\n");
			    printf(" ID--Nom--App--Apm---edad---telefono---direccion\n");
				MostrarEnfermeras(flujo_Enfermeras);
				printf("--------------------------\n");
				}
				fclose(flujo_Enfermeras);
				printf(" 1.-Modificar Enfermera\n");
			    printf(" 2.-Eliminar Enfermera\n");
			    printf(" 3.-Regresar\n");
			    scanf("%i",&cmenu2);

			    switch(cmenu2)
			    {
			    	case 1:
			    	 printf("Ingresa el ID que Quieres Modificar\n");
			    	 scanf("%i",&idenfermera);
			    	 flujo_Enfermeras=fopen("Enfermeras.bin","rb");
			    	 flujo_Enfermeras2=fopen("Enfermeras2.bin","ab");
			 		
			    	 ModificarEnfermera(flujo_Enfermeras,flujo_Enfermeras2,idenfermera);
			    	 fclose(flujo_Enfermeras);
			    	 fclose(flujo_Enfermeras2);
	  		         remove("Enfermeras.bin");
	     		     rename("Enfermeras2.bin","Enfermeras.bin");
				     break;
				     case 2:
				     printf("Ingresa el ID que Quieres Modificar\n");
			    	 scanf("%i",&idenfermera);
				     flujo_Enfermeras=fopen("Enfermeras.bin","rb");
			    	 flujo_Enfermeras2=fopen("Enfermeras2.bin","ab");
 		  	  	 	 EliminarEnfermera(flujo_Enfermeras,flujo_Enfermeras2,idenfermera);
 		  	  	 	 fclose(flujo_Enfermeras);
			    	 fclose(flujo_Enfermeras2);
	  		         remove("Enfermeras.bin");
	     		     rename("Enfermeras2.bin","Enfermeras.bin");
				     break;
			    }

			break;
			case 3:
			Alta_Paciente();
			break;
			case 4:
				
		 	    flujo_Pacientes=fopen("Pacientes.bin","rb");
				if(flujo_Pacientes==NULL)
				{
			 	 printf("Error No se encontro el archivo bin de enfermeras\n");
				}
				else
				{
			     printf("***LISTA DE PACIENTES DATOS PERSONALES***\n");
			     printf(" Cod----Nom----Apm----Apm----Genero----Edad----RESPIRADOR---CAMA\n");
				 MostrarPacientes(flujo_Pacientes);
				 printf("--------------------------\n");
				}
			    fclose(flujo_Pacientes);

			    printf(" 1.-Modificar Paciente\n");
			    printf(" 2.-Dar de Baja Paciente\n");
			    printf(" 3.-Regresar\n");
			    scanf("%i",&cmenu2);

			    switch(cmenu2)
			    {
			    	case 1:
			    	 printf("Ingresa el ID que Quieres Modificar\n");
			    	 scanf("%i",&idpaciente);
			    	 flujo_Pacientes=fopen("Pacientes.bin","rb");
			    	 flujo_Pacientes2=fopen("Pacientes2.bin","ab");
			 		
			    	 c1=ModificarPaciente(flujo_Pacientes,flujo_Pacientes2,idpaciente,0);
			    	 fclose(flujo_Pacientes);
			    	 fclose(flujo_Pacientes2);
	  		         if(c1==1)
	  		         {
	  		         printf("MODIFICACION EXITOSA\n");
	  		         remove("Pacientes.bin");
	     		     rename("Pacientes2.bin","Pacientes.bin");
	  		         }
	  		         else
	  		         {
	  		         	printf("ERROR verifica el id este dado de alta\n");
	  		         	remove("Pacientes2.bin");
	  		         }	
	  		       
				     break;
				     case 2:
				     printf("Ingresa el ID que Quieres Modificar\n");
			    	 scanf("%i",&idpaciente);
			    	 flujo_Pacientes=fopen("Pacientes.bin","rb");
			    	 flujo_Pacientes2=fopen("Pacientes2.bin","ab");
			 		
			    	 c1=EliminarPaciente(flujo_Pacientes,flujo_Pacientes2,idpaciente,0);
			    	 fclose(flujo_Pacientes);
			    	 fclose(flujo_Pacientes2);
	  		           if(c1==1)
	  		         {
	  		         printf("BAJA EXITOSA\n");
	  		         remove("Pacientes.bin");
	     		     rename("Pacientes2.bin","Pacientes.bin");
	  		         }
	  		         else
	  		         {
	  		         	printf("ERROR verifica el id este dado de alta\n");
	  		         	remove("Pacientes2.bin");
	  		         }	

				     break;
			    }

			break;
			case 5:
			Alta_Respirador();
			break;
			case 6:

			 flujo_respirador=fopen("Respiradores.bin","rb");
				if(flujo_respirador==NULL)
				{
			 	 printf("Error No se encontro el archivo bin \n");
				}
				else
				{
			    printf("***LISTA DE Respiradores Ocupados/Libres***\n");
				printf("Codigo---Modelo---ESTADO\n");
				MostrarRespiradores(flujo_respirador);
				fclose(flujo_respirador);
				flujo_respirador=fopen("Respiradores.bin","rb");
				printf("TOTAL : %i \n",ContarRespiradores(flujo_respirador,0));
				printf("--------------------------\n");
			   	fclose(flujo_respirador);	
			    printf(" 1.-Quitar Respirador\n");
				printf(" 2.-Eliminar Respirador\n");
			    printf(" 3.-Regresar\n");
			    scanf("%i",&cmenu3);
			    switch(cmenu3)
			    {
			    	case 1:
			    	printf("Ingresa el Respirador a Retirar (ID)\n");
			    	scanf("%i",&idres);
			    	 flujo_respirador=fopen("Respiradores.bin","rb");
 			     	 flujo2_respirador=fopen("Respiradores2.bin","ab");
 		  	  	 	 x=RetirarRespirador(flujo_respirador,flujo2_respirador,idres,0);
 		  	  	 	 fclose(flujo_respirador);
			    	 fclose(flujo2_respirador);
	  		         remove("Respiradores.bin");
	     		     rename("Respiradores2.bin","Respiradores.bin");
 		  	  	 	 if(x==1)
 		  	  	 	 {
 		  	  	 	 	printf(" Respirador Retirado \n");
 		  	  	 	 }
 		  	  	 	 else
 		  	  	 	 {
 		  	  	 	 	printf("EL Respirador no se encontro\n");
 		  	  	 	 }
 		  	  	 	 break;
 		  	  	 	 case 2:
 		  	  	 	 printf("Ingresa el Respirador que quieres eliminar (ID)\n");
			    	 scanf("%i",&idres);
			    	 flujo_respirador=fopen("Respiradores.bin","rb");
 			     	 flujo2_respirador=fopen("Respiradores2.bin","ab");
 		  	  	 	 EliminarRespirador(flujo_respirador,flujo2_respirador,idres);
 		  	  	 	 fclose(flujo_respirador);
			    	 fclose(flujo2_respirador);
	  		         remove("Respiradores.bin");
	     		     rename("Respiradores2.bin","Respiradores.bin");
 		  	  	 	 break;
			    }


				}
			
			break;
			case 7:
			    flujo_Cama=fopen("Camas.bin","rb");
				if(flujo_Cama==NULL)
				{
			 	 printf("Error No se encontro el archivo bin \n");
				}
				else
				{
			     printf("***Lista  de Camas Ocupadas/Libres***\n");
				 printf("Cod---Enfermera---Paciente\n");
				 MostrarCamas(flujo_Cama);
				 fclose(flujo_Cama);
				//printf("TOTAL : %i \n",ContarCamas(flujo_Cama,0));
				printf("--------------------------\n");
				}
		    break;


		    case 8:
				
		 	    flujo_Pacientes=fopen("Pacientes.bin","rb");
				if(flujo_Pacientes==NULL)
				{
			 	 printf("Error No se encontro el archivo bin de enfermeras\n");
				}
				else
				{
			     printf("***LISTA DE PACIENTES DADOS DE BAJA***\n");
			     printf(" Cod--Nom--Apm--Apm--Genero--Edad---\n");
				 MostrarPacientesEliminados(flujo_Pacientes);
				 printf("--------------------------\n");
				}
			    fclose(flujo_Pacientes);
			 break;
			 case 9:
              flujo=fopen("Pacientes.bin","rb");
              c1=ContarPacientesA(flujo,0);
              printf("Pacientes dados de Alta : %i \n",c1);
              fclose(flujo);
              flujo=fopen("Pacientes.bin","rb");
              c2=ContarPacientesB(flujo,0);
              printf("Pacientes dados de Baja : %i \n",c2);
              fclose(flujo);
              printf("TOTAL de Pacientes : %i \n",c1+c2 );
              flujo=fopen("Enfermeras.bin","rb");
              c1=ContarEnfermeras(flujo,0);
              printf("Enfermeras : %i \n",c1);
              fclose(flujo);
              flujo=fopen("Respiradores.bin","rb");
              c1=ContarRespiradores(flujo,0);
              printf("Respiradores : %i \n",c1);
              fclose(flujo);
              flujo=fopen("Camas.bin","rb");
              c1=ContarCamasTotales(flujo,0);
              printf("Total de Camas : %i \n",c1);
              fclose(flujo);
			 break;

			 case 10:
			 flujo_Enfermeras=fopen("Enfermeras.bin","rb");
    	 	 contador=ContarEnfermeras(flujo_Enfermeras,0);
    	 	 fclose(flujo_Enfermeras);
         	 p = (Enfermera*)malloc ( (contador)*sizeof(Enfermera) );
         	 flujo_Enfermeras=fopen("Enfermeras.bin","rb");
         	 ExtraerEnfermeras(flujo_Enfermeras,p,0);
         	 fclose(flujo_Enfermeras);
         	 OrdenarEnfermeras(p,contador);
         	 printf("ENFERMERA --- NCamas\n");
         	 RecorrerArrayEnfermeras(p,contador,0);  	 
			 break;
			 case 11:
			 Alta_Cama();
			 break;
			 case 12:
			 lmenu=='n';
			 break;
		}

		printf("Preciona cualquier letra para regresar al menu // Preciona la tecla n para salir \n");
		scanf(" %c",&lmenu);
		
	}
	return 0;
}