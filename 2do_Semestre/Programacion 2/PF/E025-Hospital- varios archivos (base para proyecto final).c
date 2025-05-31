#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char Nombre[50];
	char Apellidos[100];
	int Edad;
	char Direccion[100];
}Persona;

typedef struct{
	int clave;
	Persona DatosPersonales;
	char borrado;	
}Paciente;

typedef struct{
	int NumeroTrabajador;
	Persona DatosPersonales;
	char Especialidad[50];
	int Experiencia;
	char borrado;
}Doctor;

typedef struct{
	int NoConsulta;
	float costo;
	int clave;
	int NumeroTrabajador;
	char borrado;
}Consulta;

typedef struct{
	int NumeroTrabajador;
	Persona DatosPersonales;
	int NoConsulta;
	float ganancia;
}Gananciadoc;

int menu(){
	
	int opcion;
	
	system("cls");
	
	printf("\n\n\t\tHospital XYZ\n\n");
	
	printf("1. Pacientes\n");
	printf("2. Doctores\n");
	printf("3. Consultas\n");
	printf("4. Graficas\n");
	printf("5. Salir\n");
	
	printf("\nOpci�n: ");
	scanf("%d", &opcion);
	
	return opcion;
}

void ImprimePaciente(Paciente p){
	printf("\nClave: %d", p.clave);
	printf("\nNombre: %s", p.DatosPersonales.Nombre);
	printf("\nApellidos: %s", p.DatosPersonales.Apellidos);
	printf("\nEdad: %d", p.DatosPersonales.Edad);
	printf("\nDirecci�n: %s", p.DatosPersonales.Direccion);
}

void RegistroPacientes(){
	FILE *archivo;
	Paciente p;
	
	system("cls");
	
	printf("\n\t\tAlta de pacientes\n\n");

	archivo = fopen("Pacientes.bin", "ab");
	
	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
	}else{	
	
		//int resultado;
		
		printf("Clave: ");
		scanf("%d", &p.clave);
		
		printf("Nombre: ");
		fflush(stdin); gets(p.DatosPersonales.Nombre);
	
		printf("Apellidos: ");
		fflush(stdin); gets(p.DatosPersonales.Apellidos);
			
		printf("Edad: ");
		scanf("%d", &p.DatosPersonales.Edad);
		
		printf("Direccion: ");
		fflush(stdin); gets(p.DatosPersonales.Direccion);
		
		p.borrado=0;
		
		fwrite(&p, sizeof(Paciente), 1, archivo);
	
		fclose(archivo);
		
		printf("\n\nEl registro se agreg� de forma exitosa\n\n");
	}
	
	system("pause");
}

int BusquedaPacientes(int clave){
	FILE *archivo;
	Paciente p;
	int encontrado=0;
	archivo = fopen("Pacientes.bin", "rb");
	if(archivo==NULL){
		printf("Error no se pudo abrir el archivo\n");
		system("pause");
		return;
	}
	if(clave==0){
		printf("\nIngrese la clave que deasea buscar: ");
		scanf("%d", &clave);
	}
	fread(&p, sizeof(Paciente), 1, archivo);
	while(!feof(archivo) && encontrado==0){
		if(p.clave==clave && p.borrado==0){
			encontrado=1;
			ImprimePaciente(p);
			printf("\n\n");
		}
		fread(&p, sizeof(Paciente), 1, archivo);
	}
	if(encontrado==0){
		printf("No se encontro el paciente con clave %d\n\n", clave);
	}

	fclose(archivo);
	system("pause");
	
	return encontrado;
	//-------------------------------------------
	
	/* RECOMENDACI�N ***********************************
	
	Se pueden considerar dos casos:
	
	a) Si la clave llega con 0 se debe entender que en esta 
	   funci�n se debe preguntar al usuario el valor de 
	   �sta. Por lo tanto, desde el men� de pacientes se
	   deber�a llamar a esta funci�n pas�ndole el 0.
	   
	b) Si la clave llega con valor diferente a 0 significa 
	   que otra funci�n (por ejemplo, la de creaci�n de 
	   consultas) mand� la clave que desea buscar.
	****************************************************/
	
}

int patientUpdateMenu(){
	int opcion;
	printf("\n\nSelecciona el dato a modificar\n\n");
	printf("1. Nombre\n2. Apellidos\n3. Edad\n4. Direccion\n5. Salir\n");
	printf("\nOpcion: "); scanf("%d", &opcion);
	return opcion;
}

void ActualizacionPacientes(){
	FILE *archivo;
	Paciente p;
	int wantedKey, posicion, encontrado=0;
	char ans;

	system("cls");

	printf("Actualizacion de Pacientes\n");

	archivo = fopen("Pacientes.bin", "r+b");

	if (archivo == NULL){
		printf("No se pudo encontrar el archivo\n");
		system("pause");
		return; 
	}

	printf("\nIngresa la clave para actualizar: ");
	scanf("%d", &wantedKey);
	
	//Se comienza en la posicion 0 (byte 0 en el earchivo)
	posicion=0;

	fread(&p, sizeof(Paciente), 1, archivo);

	while(!feof(archivo) && encontrado==0){
		
		if(p.clave==wantedKey && p.borrado==0){
			encontrado = 1;

			ImprimePaciente(p);

			printf("\n\n¿Es el paciente que desea actualizar (s/n): ");
			fflush(stdin); scanf("%c", &ans);

			if(ans=='s' || ans=='s'){
				int opcion;

				//Se actualizan los datos
				do{
					opcion=patientUpdateMenu();

					switch(opcion){
						case 1:
							printf("Nombre: ");
							fflush(stdin); gets(p.DatosPersonales.Nombre);
							break;
						case 2:
							printf("Apellidos: ");
							fflush(stdin); gets(p.DatosPersonales.Apellidos);
							break;
						case 3:
							printf("Edad: ");
							fflush(stdin); scanf("%d", &p.DatosPersonales.Edad); 
							break;
						case 4:
							printf("Direccion: ");
							fflush(stdin); gets(p.DatosPersonales.Direccion);
							break;
					}

				}while(opcion != 5);

				fseek(archivo, sizeof(Paciente)*posicion, SEEK_SET);
				fwrite(&p, sizeof(Paciente), 1, archivo);
				printf("\nEl paciente ha sido actualizado\n");
			}else{
				printf("\nOperacion cancelada\n");
			}

		}else{
			//La posicion solo se incrementa cuando no se ha encontrado al registro.
			//Lo mismo sucede con la siguiente lectura.
			posicion++;
			fread(&p, sizeof(Paciente), 1, archivo);
		}
	}

	if(!encontrado)
		printf("\n\nNo se encontro al paciente con clave %d\n\n", wantedKey);
	
	fclose(archivo);
	system("pause");
}

void BorradoPacientes(){
	int wantedKey, posicion, encontrado;
	char ans;
	FILE *archivo;
	Paciente p;
	printf("\nIngresa la clave del paciente a borrar: ");
	scanf("%d", &wantedKey);
	archivo=fopen("Pacientes.bin", "r+b");
	if(archivo==NULL){
		printf("\nNo se pudo encontrar el archivo\n");
		system("pause");
	}
	
	//Se comineza en la posicion 0 (byte 0 en el archivo)
	posicion=0;
	fread(&p, sizeof(Paciente), 1, archivo);

	while(!feof(archivo) && encontrado==0){
		if( p.clave==wantedKey && p.borrado==0){
			encontrado=1;
			
			ImprimePaciente(p);
			
			printf("\n\n�Es el paciente que se desea borrar? (s/n): ");
			fflush(stdin); scanf("%c", &ans);
			
			if(ans=='s' || ans=='S'){
				//Se borra el registro
				p.borrado=1;

				fseek(archivo, sizeof(Paciente)*posicion, SEEK_SET);
				
				fwrite(&p, sizeof(Paciente), 1, archivo);
				
				printf("\nEl paciente ha sido borrado\n");
			}else{
				printf("\nOperaci�n cancelada\n");
			}
			
		}else{
			
			//La posici�n solo se incrementa cuando no se ha encontrado al registro.
			//Lo mismo sucede con la siguiente lectura. 
			posicion++;
			fread(&p, sizeof(Paciente), 1, archivo);
		}	
	}
}

void ListadoPacientes(){
	FILE *archivo;
	Paciente p;
	int Total=0;
	
	system("cls");
	
	printf("\n\t\tCat�logo de pacientes\n\n");

	archivo = fopen("Pacientes.bin", "rb");
	
	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
		system("pause");
		return;
	}
	
	fread(&p, sizeof(Paciente), 1, archivo);
	
	while( !feof(archivo)){
		if(p.borrado==0){
			Total++;
			ImprimePaciente(p);
			printf("\n----------------------\n");
		}
		
		fread(&p, sizeof(Paciente), 1, archivo);
	}

	printf("\n\nSe encontraron: %d pacientes\n\n", Total);
	
	fclose(archivo);
	
	system("pause");
}

void menuPaciente(){
	
	int opcion;
	
	do{
		system("cls");
		
		printf("\n\n\t\tOperaciones con pacientes\n\n");
		
		printf("1. Registro\n");
		printf("2. B�squeda\n");
		printf("3. Actualizaci�n\n");
		printf("4. Borrado\n");
		printf("5. Listado de registros\n");
		printf("6. Salir\n");
		
		printf("\nOpci�n: ");
		scanf("%d", &opcion);
		
		switch(opcion){
			case 1:
				RegistroPacientes();
				break;
			case 2:
				//Se manda con 0 para que dentro de la funci�n se
				//solicite la clave				
				BusquedaPacientes(0);
				break;
			case 3:
				ActualizacionPacientes();
				break;
			case 4:
				BorradoPacientes();
				break;
			case 5:
				ListadoPacientes();
				break;
			}
	
	}while(opcion !=6);

}

void ImprimeDoctor(Doctor d){
	printf("\nNumero de Trabajador: %d", d.NumeroTrabajador);
	printf("\nNombre: %s", d.DatosPersonales.Nombre);
	printf("\nApellidos: %s", d.DatosPersonales.Apellidos);
	printf("\nEdad: %d", d.DatosPersonales.Edad);
	printf("\nDirecci�n: %s", d.DatosPersonales.Direccion);
	printf("\nEspecialidad %s", d.Especialidad);
	printf("\nExperiencia: %d", d.Experiencia);
}

void RegistroDoctores(){
	FILE *archivo;
	Doctor d;
	
	system("cls");
	
	printf("\n\t\tAlta de doctores\n\n");

	archivo = fopen("Doctores.bin", "ab");
	
	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
	}else{	
	
		//int resultado;
		
		printf("N�mero de trabajador: ");
		scanf("%d", &d.NumeroTrabajador);
		
		printf("Nombre: ");
		fflush(stdin); gets(d.DatosPersonales.Nombre);
	
		printf("Apellidos: ");
		fflush(stdin); gets(d.DatosPersonales.Apellidos);
			
		printf("Edad: ");
		scanf("%d", &d.DatosPersonales.Edad);
		
		printf("Direccion: ");
		fflush(stdin); gets(d.DatosPersonales.Direccion);
		
		printf("Especialidad: ");
		fflush(stdin); gets(d.Especialidad);
		
		printf("Experiencia: ");
		scanf("%d", &d.Experiencia);
		
		d.borrado=0;
		
		fwrite(&d, sizeof(Doctor), 1, archivo);
	
		fclose(archivo);
		
		printf("\n\nEl registro se agreg� de forma exitosa\n\n");
	}
	
	system("pause");
}

int BusquedaDoctores(int clave){
	FILE *archivo;
	Doctor d;
	int encontrado=0;
	archivo = fopen("Doctores.bin", "rb");
	if(archivo==NULL){
		printf("Error no se pudo abrir el archivo\n");
		system("pause");
		return;
	}
	if(clave==0){
		printf("\nIngrese el numero de trabajador que deasea buscar: ");
		scanf("%d", &clave);
	}
	fread(&d, sizeof(Doctor), 1, archivo);
	while(!feof(archivo) && encontrado==0){
		if(d.NumeroTrabajador==clave && d.borrado==0){
			encontrado=1;
			ImprimeDoctor(d);
			printf("\n\n");
		}
		fread(&d, sizeof(Doctor), 1, archivo);
	}
	if(encontrado==0){
		printf("No se encontro el doctor con Numero de Trabajador %d\n\n", clave);
	}

	fclose(archivo);
	system("pause");
	
	return encontrado;
	//-------------------------------------------
	
	/* RECOMENDACI�N ***********************************
	
	Se pueden considerar dos casos:
	
	a) Si la clave llega con 0 se debe entender que en esta 
	   funci�n se debe preguntar al usuario el valor de 
	   �sta. Por lo tanto, desde el men� de pacientes se
	   deber�a llamar a esta funci�n pas�ndole el 0.
	   
	b) Si la clave llega con valor diferente a 0 significa 
	   que otra funci�n (por ejemplo, la de creaci�n de 
	   consultas) mand� la clave que desea buscar.
	****************************************************/	
}

int doctorUpdateMenu(){
	int opcion;
	printf("\n\nSelecciona el dato a modificar\n\n");
	printf("1. Nombre\n2. Apellidos\n3. Edad\n4. Direccion\n5. Especialidad\n6. Experiencia\n7. Salir\n");
	printf("\nOpcion: "); scanf("%d", &opcion);
	return opcion;
}

void ActualizacionDoctores(){
	FILE *archivo;
	Doctor d;
	int wantedKey, posicion, encontrado=0;
	char ans;

	system("cls");

	printf("Actualizacion de Doctores\n");

	archivo = fopen("Doctores.bin", "r+b");

	if (archivo == NULL){
		printf("No se pudo encontrar el archivo\n");
		system("pause");
		return; 
	}

	printf("\nIngresa el numero de trabajador para actualizar: ");
	scanf("%d", &wantedKey);
	
	//Se comienza en la posicion 0 (byte 0 en el earchivo)
	posicion=0;

	fread(&d, sizeof(Doctor), 1, archivo);

	while(!feof(archivo) && encontrado==0){
		
		if(d.NumeroTrabajador==wantedKey && d.borrado==0){
			encontrado = 1;

			ImprimeDoctor(d);

			printf("\n\n¿Es el doctor que desea actualizar (s/n): ");
			fflush(stdin); scanf("%c", &ans);

			if(ans=='s' || ans=='s'){
				int opcion;

				//Se actualizan los datos
				do{
					opcion=doctorUpdateMenu();

					switch(opcion){
						case 1:
							printf("Nombre: ");
							fflush(stdin); gets(d.DatosPersonales.Nombre);
							break;
						case 2:
							printf("Apellidos: ");
							fflush(stdin); gets(d.DatosPersonales.Apellidos);
							break;
						case 3:
							printf("Edad: ");
							fflush(stdin); scanf("%d", &d.DatosPersonales.Edad); 
							break;
						case 4:
							printf("Direccion: ");
							fflush(stdin); gets(d.DatosPersonales.Direccion);
							break;
						case 5:
							printf("Especialidad: ");
							fflush(stdin); gets(d.Especialidad);
							break;
						case 6: 
							printf("Experiencia: ");
							fflush(stdin); scanf("%d", &d.Experiencia);
							break;
					}

				}while(opcion != 7);

				fseek(archivo, sizeof(Doctor)*posicion, SEEK_SET);
				fwrite(&d, sizeof(Doctor), 1, archivo);
				printf("\nEl doctor ha sido actualizado\n");
			}else{
				printf("\nOperacion cancelada\n");
			}

		}else{
			//La posicion solo se incrementa cuando no se ha encontrado al registro.
			//Lo mismo sucede con la siguiente lectura.
			posicion++;
			fread(&d, sizeof(Doctor), 1, archivo);
		}
	}

	if(!encontrado)
		printf("\n\nNo se encontro al doctor con numeo de trabajador %d\n\n", wantedKey);
	
	fclose(archivo);
	system("pause");
}

void BorradoDoctores(){
	int wantedKey, posicion, encontrado;
	char ans;
	FILE *archivo;
	Doctor d;
	printf("\nIngresa el numero de trabajador del doctor a borrar: ");
	scanf("%d", &wantedKey);
	archivo=fopen("Doctores.bin", "r+b");
	if(archivo==NULL){
		printf("\nNo se pudo encontrar el archivo\n");
		system("pause");
	}
	
	//Se comineza en la posicion 0 (byte 0 en el archivo)
	posicion=0;
	fread(&d, sizeof(Doctor), 1, archivo);

	while(!feof(archivo) && encontrado==0){
		if( d.NumeroTrabajador==wantedKey && d.borrado==0){
			encontrado=1;
			
			ImprimeDoctor(d);
			
			printf("\n\n�Es el doctor que se desea borrar? (s/n): ");
			fflush(stdin); scanf("%c", &ans);
			
			if(ans=='s' || ans=='S'){
				//Se borra el registro
				d.borrado=1;

				fseek(archivo, sizeof(Doctor)*posicion, SEEK_SET);
				
				fwrite(&d, sizeof(Doctor), 1, archivo);
				
				printf("\nEl doctor ha sido borrado\n");
			}else{
				printf("\nOperaci�n cancelada\n");
			}
			
		}else{
			
			//La posici�n solo se incrementa cuando no se ha encontrado al registro.
			//Lo mismo sucede con la siguiente lectura. 
			posicion++;
			fread(&d, sizeof(Doctor), 1, archivo);
		}	
	}
}

void ListadoDoctores(){
	FILE *archivo;
	Doctor d;
	int Total=0;
	
	system("cls");
	
	printf("\n\t\tCat�logo de doctores\n\n");

	archivo = fopen("Doctores.bin", "rb");
	
	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
		system("pause");
		return;
	}
	
	fread(&d, sizeof(Doctor), 1, archivo);
	
	while( !feof(archivo)){
		if(d.borrado==0){
			Total++;
			ImprimeDoctor(d);
			printf("\n----------------------\n");
		}
		
		fread(&d, sizeof(Doctor), 1, archivo);
	}

	printf("\n\nSe encontraron: %d doctores\n\n", Total);
	
	fclose(archivo);
	
	system("pause");
}


void menuDoctores(){
	
	
	int opcion;
	
	do{
		system("cls");
		
		printf("\n\n\t\tOperaciones con doctores\n\n");
		
		printf("1. Registro\n");
		printf("2. B�squeda\n");
		printf("3. Actualizaci�n\n");
		printf("4. Borrado\n");
		printf("5. Listado de registros\n");
		printf("6. Salir\n");
		
		printf("\nOpci�n: ");
		scanf("%d", &opcion);
		
		switch(opcion){
			case 1:
				RegistroDoctores();
				break;
			case 2:
				
				//Se manda con 0 para que dentro de la funci�n se
				//solicite la clave
				BusquedaDoctores(0);
				break;
			case 3:
				ActualizacionDoctores();
				break;
			case 4:
				BorradoDoctores();
				break;
			case 5:
				ListadoDoctores();
				break;
			}
	
	}while(opcion !=6);
	

}

void RegistroConsultas(){
	FILE *archivo;
	Consulta c;
	
	system("cls");
	
	printf("\n\t\tAgendar consulta\n\n");

	archivo = fopen("Consultas.bin", "ab");
	
	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
	}else{	
	
		//int resultado;
		int cancela=0;
		int PacienteEncontrado;
		int MedicoEncontrado;
		
		printf("N�mero de consulta: ");
		scanf("%d", &c.NoConsulta);
		
		do{
			printf("Clave del paciente: ");
			scanf("%d", &c.clave);
			PacienteEncontrado=BusquedaPacientes(c.clave);
			
			if(PacienteEncontrado==0){
				printf("\nPaciente no encontrado �cancelar? (0=NO, 1=Si) ");
				scanf("%d", &cancela);
				
				if(cancela==1){
					printf("\nRegistro de consulta cancelada\n\n");
					system("pause");
					return;
				}
			}
		}while(PacienteEncontrado==0 && cancela==0);

		do{
			printf("Clave del m�dico: ");
			scanf("%d", &c.NumeroTrabajador);
		
			MedicoEncontrado=BusquedaDoctores(c.NumeroTrabajador);
			
			if(MedicoEncontrado==0){
				printf("\nDoctor no encontrado �cancelar? (0=NO, 1=Si) ");
				scanf("%d", &cancela);
				
				if(cancela==1){
					printf("\nRegistro de consulta cancelada\n\n");
					system("pause");
					return;
				}
			}
		}while(MedicoEncontrado==0 && cancela==0);
			
		printf("Costo de la consulta: ");
		scanf("%f", &c.costo);
		
		c.borrado=0;
		
		fwrite(&c, sizeof(Consulta), 1, archivo);
	
		fclose(archivo);
		
		printf("\n\nEl registro se agreg� de forma exitosa\n\n");
	}
	
	system("pause");
}

void ImprimeConsulta(Consulta c){
	printf("\nN�mero de la consulta: %d", c.NoConsulta);
	printf("\nDatos del paciente:");
	BusquedaPacientes(c.clave);
	
	printf("\nDatos del m�dico:");
	BusquedaDoctores(c.NumeroTrabajador);
	
	printf("\nCosto: %f", c.costo);
}

void BusquedaConsultas(){
	int consultaBuscada, encontrado=0;
	FILE *archivo;
	Consulta c;
	system("cls");
	printf("\nBusqueda de Conusltas\n");
	printf("\nIngresa el numero de la consulta que deseas buscar\n"); 
	scanf("%d", &consultaBuscada);
	archivo=fopen("Consultas.bin", "rb");
	if(archivo == NULL){
		printf("\nNo se pudo encontrar el archivo\n");
		system("pause");
		return;
	}
	fread(&c, sizeof(Consulta), 1, archivo);
	while(!feof(archivo) && encontrado==0){
		if(consultaBuscada==c.NoConsulta && c.borrado==0){
			encontrado=1;
			ImprimeConsulta(c);
			printf("\n\n");
		}
		fread(&c, sizeof(Consulta), 1, archivo);
		if(encontrado==0){
			printf("No se pudo encontrar la consulta con el numero %d", consultaBuscada);
		}
	}
	fclose(archivo);
	system("pause");
}

int consultationUpdateMenu(){
	int opcion;
	printf("\n\nSelecciona el dato a actualizar\n\n");
	printf("1. Costo\n2. Numero de Trabajador(Cambio de medico)\n3. Salir\n");
	scanf("%d", &opcion);
	return opcion;
}

void ActualizacionConsultas(){
	int consultaBuscada, found=0, posicion=0, opcion;
	char ans;
	Consulta c;
	FILE *archivo;
	system("cls");
	printf("Actualizacion de consultas\n");
	printf("Ingresa el numero de la consulta que deseas actualizar: ");
	scanf("%d", &consultaBuscada);
	archivo = fopen("Consultas.bin", "r+b");
	if(archivo==NULL){
		printf("No se pudo encontrar el archivo\n");
		system("pause");
		return;
	}
	fread(&c, sizeof(Consulta), 1, archivo);
	while(!feof(archivo) && found==0){
		if(c.NoConsulta==consultaBuscada && c.borrado==0){
			found=1;
			ImprimeConsulta(c);
			printf("\nEs la consulta que deseas modificar? s/n: ");
			fflush(stdin); scanf("%c", &ans);
			printf("\n\n");

			if(ans=='s'){

				do{
					system("cls");
					opcion=consultationUpdateMenu();

					switch(opcion){
						case 1: printf("\nCosto: "); scanf("%f", &c.costo);
						break;
						case 2: printf("\nNumero de Trabajador: "); scanf("%d", &c.NumeroTrabajador);
						break;
					}

					fseek(archivo, sizeof(Consulta)*posicion, SEEK_SET);    
					fwrite(&c, sizeof(Consulta), 1, archivo); 
				}while(opcion!=3);

				printf("\nSe actualizo correctamente la consulta\n");
				system("pause");
			}          
		}
		
		fread(&c, sizeof(Consulta), 1, archivo);
		posicion++;
		
			
		if(found==0){
			printf("\nNo se pudo encontrar la consulta con el numero %d", consultaBuscada);
		}
	}
                                                  
	  
}

void BorradoConsultas(){
	FILE *archivo;
	Consulta c;
	int Encontrado=0;
	int claveBuscada;
	char respuesta;
	int posicion;
	
	system("cls");
	
	printf("\n\t\tBorrado de Consultas\n\n");

	archivo = fopen("Consultas.bin", "r+b"); //Lectura/Escritura
	
	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
		system("pause");
		return;
	}
	
	printf("Clave a borrar: ");
	scanf("%d", &claveBuscada);
	
	//Se comienza en la posici�n 0 (byte 0 en el archivo)
	posicion=0;
	
	fread(&c, sizeof(Consulta), 1, archivo);
	
	while( !feof(archivo) && Encontrado==0){
		
		if( c.NoConsulta==claveBuscada && c.borrado==0){
			Encontrado=1;
			
			ImprimeConsulta(c);
			
			printf("\n\n�Es la consulta que se desea borrar? (s/n): ");
			fflush(stdin); scanf("%c", &respuesta);
			
			if(respuesta=='s' || respuesta=='S'){
				//Se borra el registro
				c.borrado=1;

				fseek(archivo, sizeof(Consulta)*posicion, SEEK_SET);
				
				fwrite(&c, sizeof(Consulta), 1, archivo);
				
				printf("\nLa consulta ha sido borrado\n");
			}else{
				printf("\nOperaci�n cancelada\n");
			}
			
		}else{
			
			//La posici�n solo se incrementa cuando no se ha encontrado al registro.
			//Lo mismo sucede con la siguiente lectura. 
			posicion++;
			fread(&c, sizeof(Consulta), 1, archivo);
		}	
		
	}

	if(Encontrado==0)
		printf("\n\nNo se encontr� la consulta con numero %d\n\n", claveBuscada);
	
	fclose(archivo);
	
	system("pause");
}

void ListadoConsultas(){
	FILE *archivo;
	Consulta c;
	int Total=0;
	
	system("cls");
	
	printf("\n\t\tConsultas existentes\n\n");

	archivo = fopen("Consultas.bin", "rb");
	
	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
		system("pause");
		return;
	}
	
	fread(&c, sizeof(Consulta), 1, archivo);
	
	while( !feof(archivo)){
		if(c.borrado==0){
			Total++;
			ImprimeConsulta(c);
			printf("\n----------------------\n");
		}
		
		fread(&c, sizeof(Consulta), 1, archivo);
	}

	printf("\n\nSe encontraron: %d consultas\n\n", Total);
	
	fclose(archivo);
	
	system("pause");
}

void menuConsultas(){
	
	int opcion;
	
	do{
		system("cls");
		
		printf("\n\n\t\tOperaciones con consultas\n\n");
		
		printf("1. Agendar consulta\n");
		printf("2. B�squeda\n");
		printf("3. Cambio de m�dico/costo\n");
		printf("4. Cancelaci�n de consulta\n");
		printf("5. Consultas existentes\n");
		printf("6. Salir\n");
		
		printf("\nOpci�n: ");
		scanf("%d", &opcion);
		
		switch(opcion){
			case 1:
				RegistroConsultas();
				break;
			case 2:
				BusquedaConsultas();
				break;
			case 3:
				ActualizacionConsultas();
				break;
			case 4:
				BorradoConsultas();
				break;
			case 5:
				ListadoConsultas();
				break;
			}
	
	}while(opcion !=6);

}


Gananciadoc *ganancias(){
	FILE *archivoc, *archivod;
	Consulta c;
	Doctor d;
	Gananciadoc *g, *gtemp;
	int n=0, gsize=1;

	archivoc = fopen("Consultas.bin", "rb");
	archivod = fopen("Doctores.bin", "rb");

	if(archivoc==NULL){
		printf("\nNo se pudo abrir el archivo de consultas\n");
		system("pause");
		return;
	}
	if(archivod == NULL){
		printf("\nNo se pudo abrir el archivo de doctores\n");
		system("pause");
		return;
	}


	while(!feof(archivoc)){
		fread(&c, sizeof(Consulta), 1, archivoc);
		if(c.borrado==0)
			n++;
		
	}
	rewind(archivoc);
	printf("\nCantidad de consultas: %d\n", n);
	g=(Gananciadoc *)calloc(gsize, sizeof(Gananciadoc));
	

	int isRepeated=0;
	fread(&c, sizeof(Consulta), 1, archivoc);
	for(int i=0; i<=gsize; i++){
		//GUARDAR EL PRIMER NUEMERO DE TRABAJADOR DEL ARCHIVO
	//Guardar el primer Numero de trabajador del archivo de consultas en el arreglo g de tipo Gananciadoc
	//comenzando en la posicion 0 
		if(i==0)
			g[i].NumeroTrabajador=c.NumeroTrabajador;

		if(i>=1){
			while(!feof(archivoc)){
				fread(&c, sizeof(Consulta), 1, archivoc);
		//VERIFICAR QUE EL NUMERO DE TRABAJADOR QUE SE GUARDA EN G NO ESTE REPETIDO
	//Con fread avanza una posicion el cursor del archivo y lee el siguiente numero de Trabajador del archivo 
	//por lo que a continuacion debemos verificar los numeros de trabajador guardados en g hasta ahora, uno por uno 
	//y utilizar una variable que indique si el siguiente numero de trabajador leido del archivo esta repetido 
				for(int j=0; j<=(int)gsize; j++){
					if(g[j].NumeroTrabajador==c.NumeroTrabajador) 
						isRepeated=1;
				}
		//INCREMENTAR EL TAMAÑO DEL ARREGLO G PARA GUARDAR EL SIGUINTE NUEMERO DE TRABAJADOR 
				if(isRepeated==0 && c.borrado==0){
				
					gsize++;
					gtemp=(Gananciadoc *)realloc(g, gsize*sizeof(Gananciadoc));
					g=gtemp;
					g[i].NumeroTrabajador=c.NumeroTrabajador;
					break;
				}
				isRepeated=0;
				
			}

		}		
	}

   	//EXTRAER LOS COSTOS DE LAS CONSULTAS Y ACUMULARLOS EN EL ARREGLO G EN EL NUMERO DE TRABAJADOR QUE LE CORRESPONDA
	rewind(archivoc);
	fread(&c, sizeof(Consulta), 1, archivoc);
	for(int i=0; i<gsize; ++i){
		
		while(!feof(archivoc)){
			if(g[i].NumeroTrabajador==c.NumeroTrabajador && c.borrado==0){
				g[i].ganancia+=c.costo;
			}
			if(g[i].NumeroTrabajador!=c.NumeroTrabajador){     //Cuando encuentre un numero de Trabajador que no sea igual 
				break;                                         //al que esta en la actual posicion del arreglo g[] rompe el 
			}                                                  //bucle para cambiar a la siguinte posicion del arreglo g[]  
			fread(&c, sizeof(Consulta), 1, archivoc);
		}

	}
	//EXTRAER EL NOMBRE DEL DOCTOR QUE COINCIDE CON EL NUMERO DE TRABAJADOR DEL ARCHIVO D
	fread(&d, sizeof(Doctor), 1, archivod);
	for(int i=0; i<gsize; ++i){
		while (!feof(archivod))
		{
			if(g[i].NumeroTrabajador==d.NumeroTrabajador)
			{
				strcpy(g[i].DatosPersonales.Nombre, d.DatosPersonales.Nombre);
				strcpy(g[i].DatosPersonales.Apellidos, d.DatosPersonales.Apellidos);
				break;
			}
			fread(&d, sizeof(Doctor), 1, archivod);
		}

	}
	

	//printf("\nNumero de doctores: %d\n", gsize);
	for(int i=0; i<gsize; i++){
		printf("\nNumero de trabajador(Doctor): %d", g[i].NumeroTrabajador);
		printf("\nNombre del doctor: %s %s", g[i].DatosPersonales.Nombre, g[i].DatosPersonales.Apellidos);
		printf("\nGanancias del doctor: %f\n", g[i].ganancia);
	}
	

	fclose(archivoc);
	fclose(archivod);
	free(g);
	//free(gtemp);
	g=NULL;
	gtemp=NULL;
	system("pause");
	return g;
	
}

void graficaBarras(){
	FILE *archivoc, *archivod;
	Consulta c;
	Doctor d;
	Gananciadoc *g, *gtemp;
	int n=0, gsize=1;

	archivoc = fopen("Consultas.bin", "rb");
	archivod = fopen("Doctores.bin", "rb");

	if(archivoc==NULL){
		printf("\nNo se pudo abrir el archivo de consultas\n");
		system("pause");
		return;
	}
	if(archivod == NULL){
		printf("\nNo se pudo abrir el archivo de doctores\n");
		system("pause");
		return;
	}


	while(!feof(archivoc)){
		fread(&c, sizeof(Consulta), 1, archivoc);
		if(c.borrado==0)
			n++;
		
	}
	rewind(archivoc);
	printf("\nCantidad de consultas: %d\n", n);
	g=(Gananciadoc *)calloc(gsize, sizeof(Gananciadoc));
	

	int isRepeated=0;
	fread(&c, sizeof(Consulta), 1, archivoc);
	for(int i=0; i<=gsize; i++){
		//GUARDAR EL PRIMER NUEMERO DE TRABAJADOR DEL ARCHIVO
	//Guardar el primer Numero de trabajador del archivo de consultas en el arreglo g de tipo Gananciadoc
	//comenzando en la posicion 0 
		if(i==0)
			g[i].NumeroTrabajador=c.NumeroTrabajador;

		if(i>=1){
			while(!feof(archivoc)){
				fread(&c, sizeof(Consulta), 1, archivoc);
		//VERIFICAR QUE EL NUMERO DE TRABAJADOR QUE SE GUARDA EN G NO ESTE REPETIDO
	//Con fread avanza una posicion el cursor del archivo y lee el siguiente numero de Trabajador del archivo 
	//por lo que a continuacion debemos verificar los numeros de trabajador guardados en g hasta ahora, uno por uno 
	//y utilizar una variable que indique si el siguiente numero de trabajador leido del archivo esta repetido 
				for(int j=0; j<=(int)gsize; j++){
					if(g[j].NumeroTrabajador==c.NumeroTrabajador) 
						isRepeated=1;
				}
		//INCREMENTAR EL TAMAÑO DEL ARREGLO G PARA GUARDAR EL SIGUINTE NUEMERO DE TRABAJADOR 
				if(isRepeated==0 && c.borrado==0){
				
					gsize++;
					gtemp=(Gananciadoc *)realloc(g, gsize*sizeof(Gananciadoc));
					g=gtemp;
					g[i].NumeroTrabajador=c.NumeroTrabajador;
					break;
				}
				isRepeated=0;
				
			}

		}		
	}

   	//EXTRAER LOS COSTOS DE LAS CONSULTAS Y ACUMULARLOS EN EL ARREGLO G EN EL NUMERO DE TRABAJADOR QUE LE CORRESPONDA
	rewind(archivoc);
	fread(&c, sizeof(Consulta), 1, archivoc);
	for(int i=0; i<gsize; ++i){
		
		while(!feof(archivoc)){
			if(g[i].NumeroTrabajador==c.NumeroTrabajador && c.borrado==0){
				g[i].ganancia+=c.costo;
			}
			if(g[i].NumeroTrabajador!=c.NumeroTrabajador){     //Cuando encuentre un numero de Trabajador que no sea igual 
				break;                                         //al que esta en la actual posicion del arreglo g[] rompe el 
			}                                                  //bucle para cambiar a la siguinte posicion del arreglo g[]  
			fread(&c, sizeof(Consulta), 1, archivoc);
		}

	}
	//EXTRAER EL NOMBRE DEL DOCTOR QUE COINCIDE CON EL NUMERO DE TRABAJADOR DEL ARCHIVO D
	fread(&d, sizeof(Doctor), 1, archivod);
	for(int i=0; i<gsize; ++i){
		while (!feof(archivod))
		{
			if(g[i].NumeroTrabajador==d.NumeroTrabajador)
			{
				strcpy(g[i].DatosPersonales.Nombre, d.DatosPersonales.Nombre);
				strcpy(g[i].DatosPersonales.Apellidos, d.DatosPersonales.Apellidos);
				break;
			}
			fread(&d, sizeof(Doctor), 1, archivod);
		}

	}
	

	//printf("\nNumero de doctores: %d\n", gsize);
	for(int i=0; i<gsize; i++){
		printf("\nNumero de trabajador(Doctor): %d", g[i].NumeroTrabajador);
		printf("\nNombre del doctor: %s %s", g[i].DatosPersonales.Nombre, g[i].DatosPersonales.Apellidos);
		printf("\nGanancias del doctor: %f\n", g[i].ganancia);
	}
	

	fclose(archivoc);
	fclose(archivod);
	free(g);
	//free(gtemp);
	g=NULL;
	gtemp=NULL;
	system("pause");
	return g;
	

}

void graficaPastel(){

}

void menuGraficas(){
	int opcion;
	
	
	do {
		system("cls");
		printf("\nGraficas\n\nSelecciona el tipo de grafica\n");
		printf("\n1. Grafica de Barras\n2. Grafica de Pastel\n3. Salir\n");
		scanf("%d", &opcion);

		switch(opcion){
			case 1:	graficaBarras();
				break;
			case 2:	graficaPastel();
				break;
		}

	}while(opcion!=3);
}


int main(){
	int opcion;
	
	
	do{
		opcion=menu();
		
		switch(opcion){
			case 1:
				menuPaciente();
				break;
			case 2:
				menuDoctores();
				break;
			case 3:
				menuConsultas();
				break;
			case 4:
				menuGraficas();
				break;
		}
		
	}while(opcion != 5);

	system("pause");
	return 0;
}







