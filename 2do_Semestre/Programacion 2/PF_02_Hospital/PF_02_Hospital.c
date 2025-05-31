#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> //Biblioteca necesaria para configurar idioma
#include <winbgim.h>
#include "graficas.h"

//Agregar esta función sin modificarla
void ConfiguraIdioma()
{
	/* ********************************************************************

		OBJETIVO:
		Función que configura el idioma para que los caracteres del idioma
	   	Español se vean correctamente y que las cantidades utilicen el
	   	símbolo fraccionario adecuado a México

	******************************************************************** */

	//Cambia al idioma Español
	struct lconv *lcPtr;
	setlocale(LC_ALL, "spanish");
	SetConsoleCP(1252);
    SetConsoleOutputCP(1252);
	lcPtr = localeconv();

	//Configura cantidades para México
	lcPtr->decimal_point = ".";
	lcPtr->thousands_sep = ",";
	lcPtr->grouping = "3";

}


int menu(){

	int opcion;

	system("cls");

	printf("\n\n\t\tHospital XYZ\n\n");

	printf("1. Pacientes\n");
	printf("2. Doctores\n");
	printf("3. Consultas\n");
	printf("4. Graficas\n");
	printf("5. Salir\n");

	printf("\nOpcion: ");
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
	FILE *archivo, *verificaArchivo;
	Paciente p, *p1, p1temp;
	int psize=0, repetido=0, contador=0;  //Repetido=1 -> SI, 0 -> NO
	char ans[4];


	system("cls");

	printf("\n\t\tAlta de pacientes\n\n");

	verificaArchivo = fopen("Pacientes.bin", "rb");
	archivo = fopen("Pacientes.bin", "ab");

	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
	}else if(verificaArchivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
	}else{
		//Leemos el archivo completo para saber cuantos registros contiene y asignarlo al tamaño del arreglo
		while(!feof(verificaArchivo)){
			fread(&p1temp, sizeof(Paciente), 1, verificaArchivo);
			psize++;
		}

		p1=(Paciente *)calloc(psize, sizeof(Paciente));
		rewind(verificaArchivo);
		fread(p1, sizeof(Paciente), psize, verificaArchivo);

		//int resultado;
		do{
			printf("Clave: ");
			scanf("%d", &p.clave);
			repetido=0;
			for(int i=0; i<psize; i++){
				if(p.clave==p1[i].clave && p1[i].borrado==0){
					repetido=1;
					printf("\nLa clave ingresada ya se encuentra registrada, ingrese otra por favor\n");
					system("pause");
					system("cls");
					printf("\n\t\tAlta de pacientes\n\n");
				}
			}

			if(contador>1 && strcmp(ans, "si")!=0){
				contador=0;
				printf("\n\nDeseas cancelar? si/no\n");
				fflush(stdin);
				fgets(ans, 5, stdin);
				strtok(ans, "\n");
			}
			if(strcmp(ans, "si")==0||strcmp(ans, "Si")==0||strcmp(ans, "SI")==0)
				return;
			contador++;
		}while(repetido==1);

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
		fclose(verificaArchivo);

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
		return 1;
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
	FILE *archivo;
	Paciente p;
	int wantedKey, posicion, encontrado=0;
	char ans;

	system("cls");

	printf("\n\t\tBorrado de Pacientes\n\n");

	archivo=fopen("Pacientes.bin", "r+b");

	if(archivo==NULL){
		printf("\nNo se pudo encontrar el archivo\n");
		system("pause");
		return;
	}

	printf("\nIngresa la clave del paciente a borrar: ");
	scanf("%d", &wantedKey);

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

	if(encontrado==0)
		printf("\n\nNo se encontr� al paciente con clave %d\n\n", wantedKey);

	fclose(archivo);

	system("pause");
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
		printf("2. Búsqueda\n");
		printf("3. Actualización\n");
		printf("4. Borrado\n");
		printf("5. Listado de registros\n");
		printf("6. Salir\n");

		printf("\nOpción: ");
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
	FILE *archivo, *verificaArchivo;
	Doctor d, *d1, d1temp;
	int dsize=0, repetido=0, contador=0;  //Repetido=1 -> SI, 0 -> NO
	char ans[4];

	system("cls");

	printf("\n\t\tAlta de doctores\n\n");

	verificaArchivo = fopen("Doctores.bin", "rb");
	archivo = fopen("Doctores.bin", "ab");

	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
	}else if(verificaArchivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
	}else{
		//Leemos el archivo completo para saber cuantos registros contiene y asignarlo al tamaño del arreglo
		while(!feof(verificaArchivo)){
			fread(&d1temp, sizeof(Doctor), 1, verificaArchivo);
			dsize++;
		}

		d1=(Doctor *)calloc(dsize, sizeof(Doctor));
		rewind(verificaArchivo);
		fread(d1, sizeof(Doctor), dsize, verificaArchivo);

		//int resultado;
		do{
			printf("N�mero de trabajador: ");
			scanf("%d", &d.NumeroTrabajador);
			repetido=0;
			for(int i=0; i<dsize; i++){
				if(d.NumeroTrabajador==d1[i].NumeroTrabajador && d1[i].borrado==0){
					repetido=1;
					printf("\nEl numero de trabajador ingresado ya se encuentra registrado, ingresa otro por favor\n");
					system("pause");
					system("cls");
					printf("\n\t\tAlta de doctores\n\n");
				}
			}

			if(contador>1 && strcmp(ans, "si")!=0){
				contador=0;
				printf("\n\nDeseas cancelar? si/no\n");
				fflush(stdin);
				fgets(ans, 5, stdin);
				strtok(ans, "\n");
			}
			if(strcmp(ans, "si")==0||strcmp(ans, "Si")==0||strcmp(ans, "SI")==0)
				return;
			contador++;
		}while(repetido==1);



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
		fclose(verificaArchivo);

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
		return encontrado;
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

	FILE *archivo;
	Doctor d;
	int wantedKey, posicion, encontrado=0;
	char ans;

	system("cls");

	printf("\n\t\tBorrado de Doctores\n\n");

	archivo=fopen("Doctores.bin", "r+b");

	if(archivo==NULL){
		printf("\nNo se pudo encontrar el archivo\n");
		system("pause");
		return;
	}

	printf("\nIngresa el numero de trabajador del doctor a borrar: ");
	scanf("%d", &wantedKey);

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

	if(encontrado==0)
		printf("\n\nNo se encontr� al doctor con Numero de trabajador %d\n\n", wantedKey);

	fclose(archivo);
	system("pause");
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
	FILE *archivo, *verificaArchivo;
	Consulta c, *c1, c1temp;
	int csize=0, repetido=0, contador=0;
	char ans[4];

	system("cls");

	printf("\n\t\tAgendar consulta\n\n");

	verificaArchivo = fopen("Consultas.bin", "rb");
	archivo = fopen("Consultas.bin", "ab");

	if (archivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
		system("pause");
		return;
	}else if(verificaArchivo==NULL){
		printf("\nNo se pudo abrir el archivo\n\n");
		system("pause");
		return;
	}else{

		//int resultado;
		int cancela=0;
		int PacienteEncontrado;
		int MedicoEncontrado;

		//Leemos el archivo completo para saber cuantos registros contiene y asignarlo al tamaño del arreglo
		while(!feof(verificaArchivo)){
			fread(&c1temp, sizeof(Consulta), 1, verificaArchivo);
			csize++;
		}

		c1=(Consulta *)calloc(csize, sizeof(Consulta));
		rewind(verificaArchivo);
		fread(c1, sizeof(Consulta), csize, verificaArchivo);

		//int resultado;
		do{
			printf("N�mero de consulta: ");
			scanf("%d", &c.NoConsulta);
			repetido=0;
			for(int i=0; i<csize; i++){
				if(c.NoConsulta==c1[i].NoConsulta && c1[i].borrado==0){
					repetido=1;
					printf("\nEl numero de consulta ingresado ya se encuentra registrado, ingrese otro por favor\n");
					system("pause");
					system("cls");
					printf("\n\t\tAgendar consulta\n\n");
				}
			}

			if(contador>1 && strcmp(ans, "si")!=0){
				contador=0;
				printf("\n\nDeseas cancelar? si/no\n");
				fflush(stdin);
				fgets(ans, 5, stdin);
				strtok(ans, "\n");
			}
			if(strcmp(ans, "si")==0||strcmp(ans, "Si")==0||strcmp(ans, "SI")==0)
				return;
			contador++;

		}while(repetido==1);


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
	FILE *archivo;
	Consulta c;
	int found=0, consultaBuscada, posicion=0, opcion;
	char ans;

	system("cls");

	printf("Actualizacion de consultas\n");

	archivo = fopen("Consultas.bin", "r+b");

	if(archivo==NULL){
		printf("No se pudo encontrar el archivo\n");
		system("pause");
		return;
	}

	printf("Ingresa el numero de la consulta que deseas actualizar: ");
	scanf("%d", &consultaBuscada);

	fread(&c, sizeof(Consulta), 1, archivo);

	while(!feof(archivo) && found==0){

		if(c.NoConsulta==consultaBuscada && c.borrado==0){
			found=1;

			ImprimeConsulta(c);

			printf("\nEs la consulta que deseas modificar? s/n: ");
			fflush(stdin); scanf("%c", &ans);

			printf("\n\n");

			if(ans=='s' || ans=='S'){

				do{
					system("cls");
					opcion=consultationUpdateMenu();

					switch(opcion){
						case 1: printf("\nCosto de la consulta: "); scanf("%f", &c.costo);
								printf("\nCosto modificado correctamente ...\n\n"); system("pause");
						break;
						case 2: printf("\nNumero de Trabajador: "); scanf("%d", &c.NumeroTrabajador);
								printf("\nSe cambio al medico correctamente ...\n\n"); system("pause");
						break;
					}

				}while(opcion!=3);

				fseek(archivo, sizeof(Consulta)*posicion, SEEK_SET);

				fwrite(&c, sizeof(Consulta), 1, archivo);

				printf("\nLa consulta ha sido actualizada\n");
				system("pause");
			}else{
				printf("\nOperación cancelada\n");
			}
		}else{

			//La posición solo se incrementa cuando no se ha encontrado al registro.
			//Lo mismo sucede con la siguiente lectura.
			posicion++;
			fread(&c, sizeof(Consulta), 1, archivo);
		}

	}

	if(!found)
		printf("\n\nNo se pudo encontrar la consulta con el numero: %d\n\n", consultaBuscada);

	fclose(archivo);

	system("pause");

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

void getgraphicsData(int opcion){
    FILE *archivocons, *archivodr;
	Consulta c;
	Doctor d;
	Gananciadoc *datos, *gtemp;
	int gsize=1;

    archivocons = fopen("Consultas.bin", "rb");
	archivodr = fopen("Doctores.bin", "rb");

    if(archivocons==NULL){
		printf("\nNo se pudo abrir el archivo de consultas\n");
		system("pause");
		return;
	}
	if(archivodr == NULL){
		printf("\nNo se pudo abrir el archivo de doctores\n");
		system("pause");
		return;
	}
    datos=(Gananciadoc *)calloc(gsize, sizeof(Gananciadoc));

    int isRepeated=0;
	fread(&c, sizeof(Consulta), 1, archivocons);

	for(int i=0; i<=gsize; i++){
	    //GUARDAR EL PRIMER NUEMERO DE TRABAJADOR DEL ARCHIVO
	    //Guardar el primer Numero de trabajador del archivo de consultas en el arreglo datos de tipo Gananciadoc
	    //comenzando en la posicion 0
		while(!feof(archivocons)&&c.borrado==1){
				fread(&c, sizeof(Consulta), 1, archivocons);
				//fseek(archivocons, sizeof(Consulta), SEEK_CUR);
			}
		if(i==0){
			datos[i].NumeroTrabajador=c.NumeroTrabajador;
		}


		if(i>=1){
			while(!feof(archivocons)){
				fread(&c, sizeof(Consulta), 1, archivocons);
		//VERIFICAR QUE EL NUMERO DE TRABAJADOR QUE SE GUARDA EN datos NO ESTE REPETIDO
	    //Con fread avanza una posicion el cursor del archivo y lee el siguiente numero de Trabajador del archivo
	    //por lo que a continuacion debemos verificar los numeros de trabajador guardados en g hasta ahora, uno por uno
	    //y utilizar una variable que indique si el siguiente numero de trabajador leido del archivo esta repetido
				for(int j=0; j<=(int)gsize; j++){
					if(datos[j].NumeroTrabajador==c.NumeroTrabajador)
						isRepeated=1;
				}
		//INCREMENTAR EL TAMAÑO DEL ARREGLO datos PARA GUARDAR EL SIGUINTE NUEMERO DE TRABAJADOR
				if(isRepeated==0 && c.borrado==0){

					gsize++;
					gtemp=(Gananciadoc *)realloc(datos, gsize*sizeof(Gananciadoc));
					//     //dos formas de copiar arreglos de tipo derivado la de abajo rompio el programa
					//free(g);
					//g=(Gananciadoc *)calloc(gsize, sizeof(Gananciadoc));
					//memcpy(g, gtemp, gsize*sizeof(Gananciadoc));
					datos=gtemp;
					datos[i].NumeroTrabajador=c.NumeroTrabajador;
					break;
				}
				isRepeated=0;

			}

		}
	}

    //EXTRAER LOS COSTOS DE LAS CONSULTAS Y ACUMULARLOS EN EL ARREGLO datos EN EL NUMERO DE TRABAJADOR QUE LE CORRESPONDA
	int savedPosition;
	rewind(archivocons);
	fread(&c, sizeof(Consulta), 1, archivocons);

	while(c.borrado==1){
		fread(&c, sizeof(Consulta), 1, archivocons);
	}

	//savedPosition=ftell(archivocons);
	printf("\nSaved Position: %d\n", savedPosition);

	for(int i=0; i<gsize; ++i){

		datos[i].ganancia=0;

		while(!feof(archivocons)){
			if(datos[i].NumeroTrabajador==c.NumeroTrabajador && c.borrado==0){
				datos[i].ganancia+=c.costo;
			}
			//if(datos[i].NumeroTrabajador!=c.NumeroTrabajador){     //Cuando encuentre un numero de Trabajador que no sea igual
				//break;                                         //al que esta en la actual posicion del arreglo datos[] se rompe el
			//}                                                  //bucle para cambiar a la siguinte posicion del arreglo datos[]
			fread(&c, sizeof(Consulta), 1, archivocons);
		}

		//fseek(archivocons, savedPosition, SEEK_SET);
		rewind(archivocons);
		fread(&c, sizeof(Consulta), 1, archivocons);
	}

    //EXTRAER EL NOMBRE DEL DOCTOR QUE COINCIDE CON EL NUMERO DE TRABAJADOR DEL ARCHIVO D
	fread(&d, sizeof(Doctor), 1, archivodr);

	for(int i=0; i<gsize; ++i){

		rewind(archivodr);
		while (!feof(archivodr))
		{
			if(datos[i].NumeroTrabajador==d.NumeroTrabajador && d.borrado==0)
			{
				strcpy(datos[i].DatosPersonales.Nombre, d.DatosPersonales.Nombre);
				strcpy(datos[i].DatosPersonales.Apellidos, d.DatosPersonales.Apellidos);
				break;
			}
			fread(&d, sizeof(Doctor), 1, archivodr);
		}

	}

	for(int i=0; i<gsize; i++){
		printf("\nNumero de trabajador(Doctor): %d", datos[i].NumeroTrabajador);
		printf("\nNombre del doctor: %s %s", datos[i].DatosPersonales.Nombre, datos[i].DatosPersonales.Apellidos);
		printf("\nGanancias del doctor: %f\n", datos[i].ganancia);
	}

	if(opcion==1){
		graficaBarras(datos, gsize);
	}else if(opcion==2){
		graficaPastel(datos, gsize);
	}

	system("pause");
    fclose(archivocons);
	fclose(archivodr);
	//free(datos);
    return;

}


void menuGraficas(){
	int opcion;
	do {
		system("cls");
		printf("\nGraficas\n\nSelecciona el tipo de grafica\n");
		printf("\n1. Grafica de Barras\n2. Grafica de Pastel\n3. Salir\n");
		printf("\nopcion: ");
		scanf("%d", &opcion);

		getgraphicsData(opcion);

	}while(opcion!=3);
}


int main(){
	int opcion;
	FILE *caratula;
	char c;

	//Configura el idioma desde el inicio de main. Llamar a la función.
	ConfiguraIdioma();

	caratula = fopen("Portada.txt", "r");

	if(caratula==NULL){
		printf("\n\n\t\tError al abrir el archivo");
		return 1;
	}else{
		while((c=fgetc(caratula))!=EOF){
			printf("%c", c);
		}
		printf("\n\n");
	}
	system("pause");


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







