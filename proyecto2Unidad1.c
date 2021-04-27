#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void mdb(char *nombre, int cap);
bool ldb(char *BaseDatos);
void mreg(int cedula, char *nombre, int semestre);
void rr(int cedula);
void svdb(char *nombre);
void radb();
void rsdb();
void lsdbs();
void gdb();
bool salir();
void sdb(char *nombre);

typedef struct Estudiante
{
  int cedula;
  char nombre[30];
  int semestre;
} Estudiante;

static int activado = 1;
static int capacidad = 0, cantidad = 0;
Estudiante *dato;

int main()
{

  char comando[10], parametro1[20], parametro2[20], parametro3[10], linea[50];
  bool salir1 = false, salir2 = false;

  do
  {

    printf("Linea de comandos de la interfaz DB\n");
    printf("-------------Menu Principal-------------\n");
    printf("> ");
    fgets(linea, 50, stdin);
    sscanf(linea, "%s %s %s %s", comando, parametro1, parametro2, parametro3);

    if (strstr(comando, "mdb") != NULL)
    {
      mdb(parametro1, atoi(parametro2));
    }
    else if (strstr(comando, "ldb") != NULL)
    {

      if (ldb(parametro1) == false)
      {

        printf("Base de Datos inexistente\n");
      }
      else
      {
        printf("Base de Datos cargada correctamente\n");
        do
        {

          printf("-------------Menu de la Base de Datos-------------\n");
          printf("> ");
          fgets(linea, 50, stdin);
          sscanf(linea, "%s %s %s %s", comando, parametro1, parametro2, parametro3);
          if (strstr(comando, "gdb") != NULL)
          {

            gdb();
          }
          else if (strstr(comando, "lsdbs") != NULL)
          {

            lsdbs();
          }
          else if(strstr(comando, "sdb") != NULL)
          {
            sdb(parametro1);
          }
          else if (strstr(comando, "mdb") != NULL)
          {

            mdb(parametro1, atoi(parametro2));
          }
          else if (strstr(comando, "ldb") != NULL)
          {

            if (ldb(parametro1) == false)
            {

              printf("Base de Datos inexistente\n");
            }
          }
          else if (strstr(comando, "exit") != NULL)
          {

            salir2 = salir();
          }

        } while (salir2 == false);
        salir2 = false;
      }
    }
    else if (strstr(comando, "exit") != NULL)
    {

      salir1 = true;
    }

  } while (salir1 == false);
  return 0;
}

void mdb(char *nombre, int cap)
{

  int cant = 0;
  FILE *metaDatos = fopen("metaDatos.txt", "a");

  if (metaDatos == NULL)
  {
    perror("Error en la apertura del archivo");
  }
  else
  {
    fprintf(metaDatos, "%s %d %d\n", nombre, cap, cant);
    fflush(metaDatos);
    fclose(metaDatos);
    printf("Base de datos %s se ha creado exitosamente\n", nombre);
  }
}

bool ldb(char *BaseDatos)
{

  char nombre[50], linea[50], opcion, capta;
  bool existe = false;
  FILE *metaDatos = fopen("metaDatos.txt", "r");

  if (activado == 0)
  {
    printf("Tiene una base de datos activada ¿Desea cerrarla?\nS/N\nIngrese la letra: ");
    scanf("%c", &opcion);
    scanf("%c", &capta);

    if (opcion == 'n' || opcion == 'N')
    {
      return false;
    }
    else if (opcion == 's' || opcion == 'S')
    {
      printf("¿Desea guardar los datos?\nS/N\nIngrese la letra: ");
      scanf("%c", &opcion);
      scanf("%c", &capta);

      if (opcion == 'S' || opcion == 's')
      {
        printf("Ingrese el nombre con la que desea guardarlo\n");
        fgets(linea, 50, stdin);
        sscanf(linea, "%s", nombre);
        svdb(nombre);
      }

      activado = 1;
    }
  }
  else if (activado == 1)
  {
    while (feof(metaDatos) == 0)
    {
      fscanf(metaDatos, "%s %d %d", nombre, &capacidad, &cantidad);

      if (strcmp(BaseDatos, nombre) == 0)
      {
        existe = true;
        activado = 0;
        free(dato);
        dato = NULL;
        dato = (Estudiante *)malloc(sizeof(Estudiante) * capacidad);

        if (cantidad > 0)
        {
          FILE *BaseDatos = fopen(nombre, "r");
          int ced, sem;
          char nom[30];
          while (feof(BaseDatos) == 0)
          {

            for (int i = 0; i < capacidad; i++)
            {
              fscanf(BaseDatos, "%d %s %d", &ced, nom, &sem);
              (dato + i)->cedula = ced;
              strncpy((dato + i)->nombre, nom, 30);
              (dato + i)->semestre = sem;
            }
          }

          fclose(BaseDatos);
        }

        fclose(metaDatos);
        break;
      }
    }
  }

  return existe;
}

void lsdbs()
{

  FILE *metaDatos = fopen("metaDatos.txt", "r");
  char caracter;

  if (metaDatos == NULL)
  {

    printf("\nError de apertura del archivo. \n\n");
  }
  else
  {
    printf("\nLas bases de datos creadas son:  \n\n");
    while ((caracter = fgetc(metaDatos)) != EOF)
    {
      printf("%c", caracter);
    }
  }
  fclose(metaDatos);
}
FILE *sync;

void gdb()
{

  int canregis;

  canregis = capacidad - cantidad;

  char line[1024] = {
      0,
  };

  sync = fopen("metaDatos.txt", "r");
  if (sync)
  {
    while (fgets(line, 1024, sync) != NULL)
    {
    }

    printf("La base de datos actual con su tamano es: %s\n", line);
    printf("Cantidad de datos disponibles = %d\n", canregis);
    fclose(sync);
  }
}

void radb()
{

  if (activado == 0)
  {
    for (int i = 0; i < cantidad; i++)
    {
      printf("%d   %s   %d\n", (dato + i)->cedula, (dato + i)->nombre, (dato + i)->semestre);
    }
  }
  else
  {
    printf("Debe tener una base de datos cargada para realizar esta accion\n");
  }
}

void rsdb()
{

  if (activado == 0)
  {
    printf("Capacidad de la Base de Datos = %d\nCantidad de datos actuales = %d\n", capacidad, cantidad);
  }
  else
  {
    printf("Debe tener una base de datos cargada para realizar esta accion\n");
  }
}

void mreg(int cedula, char *nombre, int semestre)
{

  if (activado == 0)
  {

    if (cantidad < capacidad)
    {
      (dato + cantidad)->cedula = cedula;
      strncpy((dato + cantidad)->nombre, nombre, 30);
      (dato + cantidad)->semestre = semestre;
      cantidad++;
      printf("Informacion guardada correctamente\n");
    }
    else
    {
      printf("Memoria llena, excede la cantidad de registros\n");
    }
  }
  else
  {
    printf("Debe tener una base de datos cargada para realizar esta accion\n");
  }
}

void rr(int cedula)
{

  if (activado == 0)
  {
    for (int i = 0; i < capacidad; i++)
    {
      if ((dato + i)->cedula == cedula)
      {
        printf("%d   %s   %d\n", (dato + i)->cedula, (dato + i)->nombre, (dato + i)->semestre);
        break;
      }
    }
  }
  else
  {
    printf("Debe tener una base de datos cargada para realizar esta accion\n");
  }
}

void svdb(char *nombre)
{

  bool existe = false;
  int capacidadLocal, cantidadLocal;
  FILE *guardar = fopen(nombre, "a");
  FILE *metaDatos = fopen("metaDatos.txt", "r+");
  char BaseDatos[50];

  if (activado == 1)
  {
    printf("Debe tener una base de datos cargada para realizar esta accion\n");
    return;
  }

  if (guardar == NULL)
  {
    perror("Error en la apertura del archivo");
    return;
  }

  if (metaDatos == NULL)
  {
    perror("Error en la apertura del archivo");
  }

  for (int i = 0; i < cantidad; i++)
  {
    fprintf(guardar, "%d   %s   %d\n", (dato + i)->cedula, (dato + i)->nombre, (dato + i)->semestre);
  }

  fflush(guardar);
  fclose(guardar);
  printf("Base de datos guardada exitosamente\n");

  while (feof(metaDatos) == 0)
  {
    fscanf(metaDatos, "%s%d%d", BaseDatos, &capacidadLocal, &cantidadLocal);
    if (strcmp(BaseDatos, nombre) == 0)
    {
      existe = true;
      break;
    }
  }

  if (existe == false)
  {
    fprintf(metaDatos, "%s %d %d\n", nombre, capacidad, cantidad);
  }

  fflush(metaDatos);
  fclose(metaDatos);
}

bool salir()
{

  char nombre[50], linea[50], opcion, capta;

  if (activado == 0)
  {

    printf("Advertencia: Ingresar una opcion inexistente no realizaria la accion deseada\n");
    printf("Tiene una base de datos activada ¿Desea cerrarla?\nS/N\nIngrese la letra: ");
    scanf("%c", &opcion);
    scanf("%c", &capta);

    if (opcion == 'n' || opcion == 'N')
    {
      activado = 0;
      return false;
    }
    else if (opcion == 's' || opcion == 'S')
    {
      printf("Advertencia: Ingresar una opcion inexistente no realizaria la accion deseada\n");
      printf("¿Desea guardar los datos?\nS/N\nIngrese la letra: ");
      scanf("%c", &opcion);
      scanf("%c", &capta);

      if (opcion == 'S' || opcion == 's')
      {
        printf("Ingrese el nombre con la que desea guardarlo\n");
        fgets(linea, 50, stdin);
        sscanf(linea, "%s", nombre);
        svdb(nombre);
      }

      free(dato);
      dato = NULL;
      activado = 1;
      return true;
    }
  }

  return false;
}


void sdb(char *nombre)
{
  char comando[10], parametro1[20], parametro2[20], parametro3[10], linea[50];

  printf("entraste a sdb\n");
  do
  {

    printf("-------------Menu de %s-------------\n", nombre);
    printf("comandos: svdb, radb, rsdb, mreg, rr y exit\n");
    printf("> ");
    fgets(linea, 50, stdin);
    sscanf(linea, "%s %s %s %s", comando, parametro1, parametro2, parametro3);
    if (strstr(comando, "exit") != NULL)
    {
      printf("saliste de la base de datos %s\n", nombre);
      break;
    }
    else if (strstr(comando, "radb") != NULL)
    {
      radb();
    }
    else if (strstr(comando, "rsdb") != NULL)
    {
      rsdb();
    }
    else if (strstr(comando, "mreg") != NULL)
    {
      mreg(atoi(parametro1), parametro2, atoi(parametro3));
    }
    else if (strstr(comando, "rr") != NULL)
    {
      rr(atoi(parametro1));
    }
    else if (strstr(comando, "svdb") != NULL)
    {
      svdb(nombre);
    }

  } while (true);
}
