#include "list.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


List* ListaGlobal = NULL;

typedef struct {
  char tipo[30];
  char nombreI[30];
  int nivel_o_cantidad;
  char nombreP[30];
} Item;

typedef struct {
  List *Items;
  char nombrePe[30];
  int cantidad;
} Personajes;

// Prototipos

void AgregarItem(List *, char *, char *, int, char *,List *);
void ImportarItem(List *,List *,char *);
char *get_csv_field(char *, int);
void MostrarNombresPersonajes(List *);
void MostrarTodosLosItems(List *);
void MostrarItemsEquipables(List *list);
void MostrarItemsConsumibles(List *list);
void eliminarItemdeUnPersonaje(char *, char *, int, char*, List *, List *); 
void eliminarItemPersonajes(char * , char * ,List *, List *);
void exportar(List *,char *);
FILE *archItems;

// MAIN

int main() {
  //Declaracion de variables
  char tipo[30];
  char nombreI[30]; 
  int nivel_o_cantidad;
  char nombreP[30];
  char archivo[100];

  //Declaracion y reservacion de memoria para listas y  variable tipo "Item".
  Item *personajes = (Item *)malloc(sizeof(Item));
  List *personaje = createList(); // lista general
  List *ListaGlobal = createList();


  int opcion;

  while (opcion != 10) {
    printf("---------------  MENÚ  ---------------\n");
    printf("1. -Importar Ítems\n");
    printf("2. -Exportar Ítems\n");
    printf("3. -Agregar Ítem \n");
    printf("4. -Mostrar ítems Equipables\n");
    printf("5. -Mostrar ítems Consumibles\n");
    printf("6. -Eliminar ítems de un personaje\n");
    printf("7. -Eliminar ítem de todos los personajes\n");
    printf("8. -Mostrar nombres de los personajes\n");
    printf("9. -Mostrar todos los ítems\n");
    printf("10.-Salir\n");
    printf("--------------------------------------\n");
    printf("Indica la opcion: ");
    scanf("%d", &opcion);
    printf("\n");

    switch (opcion) 
    {
      case 1: 
      {
        printf("1. --- Importando Ítems ---\n");
        printf("Ingrese el nombre del archivo \n");
        getchar();
        scanf("%100[^\n]s", archivo);
        ImportarItem(personaje,ListaGlobal,archivo);
        break;
      }
      case 2: 
      {
        printf("2. --- Exportar Ítems ---\n");
        printf("Ingrese el nombre del archivo que desea crear (o sobreescribir) \n");
        getchar();
        scanf("%100[^\n]s", archivo);
        exportar(personaje,archivo);
        break;
      }
      case 3: 
      {
        printf("3. ---Agregar Item--- \n");
        printf("Ingrese el tipo del Item \n");
        getchar();
        scanf("%30[^\n]s", tipo);
        getchar();
        printf("Ingrese el nombre del Item \n");
        scanf("%30[^\n]s", nombreI);
        getchar();
        printf("Ingrese el nivel o cantidad del Item \n");
        scanf("%d", &nivel_o_cantidad);
        getchar();
        printf("Ingrese el personaje al que le va a agregar el Item \n"); //CARACTERES, NUMEROS SOLOS NO.
        scanf("%30[^\n]s", nombreP);
        getchar();
        AgregarItem(personaje, tipo, nombreI, nivel_o_cantidad, nombreP,ListaGlobal);
        break;
      }
      case 4: 
      {
        printf("4. ---Mostrar Ítems Equipables---\n ");
        MostrarItemsEquipables(personaje);
        break;
      }
      case 5: 
      {
        printf("5. ---Mostrar Ítems Consumibles---\n ");
        MostrarItemsConsumibles(personaje);
        break;
      }
      case 6: 
      {
        printf("6. ---Eliminar un ítem de un personaje---\n");
        printf("Ingrese el tipo del Item a eliminar\n");
        getchar();
        scanf("%30[^\n]s", tipo);
        getchar();
        printf("Ingrese el nombre del Item a eliminar\n");
        scanf("%30[^\n]s", nombreI);
        getchar();
        if(strcmp(tipo,"Consumible") == 0)
        {
          printf("Ingrese la cantidad del Item que quiere eliminar\n");
          scanf("%d", &nivel_o_cantidad);
          getchar();
        }
        printf("Ingrese el nombre del personaje al que le eliminará el Item\n");
        scanf("%30[^\n]s", nombreP);
        getchar();
        eliminarItemdeUnPersonaje(tipo, nombreI, nivel_o_cantidad, nombreP, ListaGlobal, personaje);
        break;
      }
      case 7: 
      {
        printf("7. ---Eliminar un ítem de todos los personajes---\n ");
        printf("Ingrese el tipo del Item a eliminar\n");
        getchar();
        scanf("%30[^\n]s", tipo);
        getchar();
        printf("Ingrese el nombre del Item a eliminar\n");
        scanf("%30[^\n]s", nombreI); 
        getchar();
        eliminarItemPersonajes(tipo, nombreI, personaje, ListaGlobal);
        break;
      }
      case 8: 
      {
        printf("8. ---Mostrar nombres de los personajes---\n");
        MostrarNombresPersonajes(ListaGlobal);
        break;
      }
      case 9:
      {
        printf("9. ---Mostar todos los Items---\n");
        MostrarTodosLosItems(personaje);
        break;
      }
      case 10: 
      {
        printf("---Saliendo del programa---\n");
        break;
      }
    }
    printf("\n");
  }
  return 0;
}

// Para leer el archivo
char *get_csv_field(char *tmp, int k) {
  int open_mark = 0;
  char *ret = (char *)malloc(100 * sizeof(char));
  int ini_i = 0, i = 0;
  int j = 0;
  while (tmp[i + 1] != '\0') {

    if (tmp[i] == '\"') {
      open_mark = 1 - open_mark;
      if (open_mark)
        ini_i = i + 1;
      i++;
      continue;
    }

    if (open_mark || tmp[i] != ',') {
      if (k == j)
        ret[i - ini_i] = tmp[i];
      i++;
      continue;
    }

    if (tmp[i] == ',') {
      if (k == j) {
        ret[i - ini_i] = 0;
        return ret;
      }

      j++;
      ini_i = i + 1;
    }

    i++;
  }

  if (k == j) {
    ret[i - ini_i] = 0;
    return ret;
  }
  return NULL;
}


void MostrarTodosLosItems(List *list) {
  Item *aux = firstList(list);
  while (aux != NULL) {
    printf("%s\n", aux->nombreI);
    aux = nextList(list);
  }
}

void MostrarItemsEquipables(List *list) {
  Item *aux = firstList(list);
  while (aux != NULL) {
    if(strcmp(aux->tipo,"Equipable") == 0 ){
      printf("%s\n", aux->nombreI);
    }
    aux = nextList(list);
  }
}

void MostrarItemsConsumibles(List *list) {
  Item *aux = firstList(list);
  while (aux != NULL) {
    if(strcmp(aux->tipo,"Consumible") == 0 ){
      printf("%s\n", aux->nombreI);
    }
    aux = nextList(list);
  }
}


void MostrarNombresPersonajes(List *ListaGlobal) {
  
  Personajes *aux = firstList(ListaGlobal);
  if(aux == NULL)
  {
    printf("No existe(n) el/los personajes"); 
    return;
  }
  else{
    while(aux != NULL) {
      if(aux->cantidad == 1)
      {
       printf("%s contiene 1 Item\n", aux->nombrePe);
      }
      if (aux->cantidad <= 0)
      {
        printf("%s no contiene ningún Item\n", aux->nombrePe);
      }
      if (aux->cantidad > 1)
      {
        printf("%s contiene %i Items\n", aux->nombrePe, aux->cantidad);
      }
      aux = nextList(ListaGlobal);
    }
  }
}

// Importar Items

void ImportarItem(List *personaje, List *listag, char* archivo) {

  char *aux = (char*)malloc(sizeof(char));
  FILE *archItems = fopen(archivo, "r");
  
  if(archItems != NULL)
  {
    printf("Su archivo ha sido cargado con éxito\n");
  }
  else
  {
    printf("Archivo no encontrado\n");
    return;
  }
  
  char linea[200];
  int i = 0;
  int k = 0;
  bool flag = true;
  
  fgets(linea, 200, archItems);
  while (fgets(linea, 200, archItems) != NULL) 
  {
    linea[strlen(linea) - 1] = 0; //Se elimina el salto de línea.

    while (flag != false) {
      Item *Items = (Item *)malloc(sizeof(Item));
      for (i = 0; i < 4; i++) {
        char *aux = get_csv_field(linea,i);
        if (i == 0) 
        {
          strcpy(Items->tipo, aux);
        }
        if (i == 1) 
        {
          strcpy(Items->nombreI, aux);
        }
        if (i == 2) 
        {
          Items->nivel_o_cantidad = atoi(aux); //se cambia el str a int
        }
        if (i == 3) 
        {
          strcpy(Items->nombreP, aux);
        }
        
      }
      flag = false;
      AgregarItem(personaje, Items->tipo, Items->nombreI, Items->nivel_o_cantidad, Items->nombreP,listag);
      
    }
    
    i++;
    i = 0;
    flag = true;
    k++;
  }
  
  fclose(archItems);
}

void AgregarItem(List *list, char *tipo, char *nombreI, int nivel_o_cantidad, char *nombreP,List *ListaGlobal) {
 
  Personajes* PersonajeN;
  Item* ItemAgregado = (Item*)malloc(sizeof(Item));
  Item* aux; //VARIABLE AUXILIAR PARA LUEGO IR COMPARANDO.
  
  //EL USUARIO INGRESA LOS DATOS DE UN ITEM.
  strcpy(ItemAgregado->nombreI, nombreI);
  strcpy(ItemAgregado->tipo,tipo);
  ItemAgregado->nivel_o_cantidad = nivel_o_cantidad;
  strcpy(ItemAgregado->nombreP, nombreP);
  
  PersonajeN = firstList(ListaGlobal); //LISTA QUE CONTIENE "LISTAS" DE LOS PERSONAJES.

  while(PersonajeN != NULL)
  {
    if(strcmp(PersonajeN->nombrePe,nombreP) == 0) //SE BUSCA SI LA LISTA YA EXISTE
    {
      break;
    }
    else
    {
      PersonajeN = nextList(ListaGlobal);
    }
  }

  //SI UN PERSONAJE NO EXISTE, DEBERÁ CREAR UNO NUEVO.
  if(PersonajeN == NULL)
  {
    PersonajeN = (Personajes*)malloc(sizeof(Personajes));
    strcpy(PersonajeN->nombrePe,nombreP);
    PersonajeN->cantidad = 0;
    PersonajeN->Items = createList();
    pushBack(ListaGlobal, PersonajeN);
  }

  if(firstList(PersonajeN->Items))
  { 
    aux = firstList(PersonajeN->Items);
    while(aux != NULL)
    {
      if(strcmp(ItemAgregado->nombreP,nombreP) == 0) //COMPARAMOS LOS DATOS DEL ITEM
      { 
        if(strcmp(aux->nombreI,nombreI) == 0)
        { 
          if(strcmp(aux->tipo,tipo) == 0)
          {
            aux->nivel_o_cantidad++; 
          }
          else
          {
            printf("\nNo puede ingresar un Consumible con el nombre de un Equipable y viceversa.\n");
            return;
          }
        }
      }
      aux = nextList(PersonajeN->Items);
    }
  }
  else //SI NO EXISTE EL ITEM SE AGREGA.
  {
    pushFront(PersonajeN->Items, ItemAgregado);
    PersonajeN->cantidad++;
    pushBack(list, ItemAgregado);
    
    return;
  }
  PersonajeN->cantidad++;
  pushBack(PersonajeN->Items, ItemAgregado); //AGREGA LOS ITEMS A LAS DOS LISTAS.
  pushBack(list, ItemAgregado);
  
}

//FUNCIÓN ELIMINAR 1 ITEM DE 1 PERSONAJE.
void eliminarItemdeUnPersonaje(char *tipo, char *nombreI, int nivel_o_cantidad, char* nombreP, List *ListaGlobal, List *personaje) {
  
  Personajes *PersonajeN = firstList(ListaGlobal);
  while (PersonajeN != NULL) 
  {
    Item *Itemaux = firstList(PersonajeN->Items);
    while (Itemaux != NULL) 
    {
      if (strcmp(Itemaux->tipo, tipo) == 0 && strcmp(PersonajeN->nombrePe, nombreP) == 0 && strcmp(Itemaux->nombreI, nombreI) == 0) //SE COMPARAN LOS DATOS
      {
        if (strcmp(Itemaux->tipo, "Consumible") == 0) //SI ES CONSUMIBLE SE LE RESTA LA CANTIDAD QUE INGRESÓ A LA CANTIDAD QUE YA HABÍA.
        {
          Itemaux->nivel_o_cantidad = Itemaux->nivel_o_cantidad - nivel_o_cantidad;
          if ((Itemaux->nivel_o_cantidad) <= 0) 
          {
            Item* datosI = firstList(personaje); //SE ELIMINA DE LA LISTA GENERAL
            while(datosI != NULL)
            {
              if(strcmp(datosI->tipo, tipo)==0 && strcmp(datosI->nombreI, nombreI) == 0 && strcmp(PersonajeN->nombrePe,nombreP) == 0)
              {
                popCurrent(personaje);
              }
            datosI=nextList(personaje);
            }
            popCurrent(PersonajeN->Items); //SE ELIMINA DE LA LISTA DE ITEMS.
            PersonajeN->cantidad--;
            printf("Item eliminado correctamente\n");
            return;
          }
          else
          {
            Itemaux->nivel_o_cantidad--;
            return;
          }
        } 
        else //SI ES EQUIPABLE SE ELIMINA DE LAS 2 LISTAS DIRECTAMENTE
        {
          Item* datosI = firstList(personaje);
          while(datosI != NULL)
          {
            if(strcmp(datosI->tipo, tipo)==0 && strcmp(datosI->nombreI, nombreI) == 0 && strcmp(PersonajeN->nombrePe,nombreP) == 0)
            {
              popCurrent(personaje);
            }
          datosI=nextList(personaje);
          }
          popCurrent(PersonajeN->Items);
          PersonajeN->cantidad--;
          printf("Item eliminado correctamente\n");
          return;
        }
      }
      Itemaux = nextList(PersonajeN->Items);
    }
    PersonajeN = nextList(ListaGlobal);
  }
  printf("No existe ningún ítem que coincida con los datos ingresados.\n");
}

//FUNCIÓN ELIMINAR 1 ITEM DE TODOS LOS PERSONAJES.
void eliminarItemPersonajes(char* tipo, char* nombreI, List* personaje, List* ListaGlobal)
{
 Personajes*  PersonajeN= firstList(ListaGlobal);
 while (PersonajeN != NULL)
  {
    Item* Itemaux = firstList(PersonajeN->Items);
    while (Itemaux != NULL)
    { //SE COMPARAN LOS NOMBRES Y SE ELIMINA DE LAS 2 LISTAS EL ITEM.
      if (strcmp(Itemaux->tipo, tipo) == 0 && strcmp(Itemaux->nombreI, nombreI) == 0) 
      {
        Item* datosI = firstList(personaje);
        while(datosI != NULL)
        {
          if(strcmp(datosI->tipo, tipo)==0 && strcmp(datosI->nombreI, nombreI) == 0)
          {
            popCurrent(personaje);
          }
        datosI=nextList(personaje);
        }
        popCurrent(PersonajeN->Items); 
        PersonajeN->cantidad--; 
        printf("Item eliminado correctamente\n");
        return;
      }
      Itemaux = nextList(PersonajeN->Items);
    }
    PersonajeN = nextList(ListaGlobal);
  }
  printf("No existe ningún ítem que coincida con los datos ingresados.\n");
}

void exportar(List *personaje, char* archivo)
{
  FILE *archItems = fopen(archivo, "wt");
  if (archItems == NULL)
  {
    printf("No se pudo abrir el archivo");
    return;
  }
  Item* Archivo2 = firstList(personaje); 
  while (Archivo2 != NULL) //MIENTRAS EL ARCHIVO NO SEA NULO SE AGREGAN LOS DATOS DE LOS ITEMS.
  {
    fputs(Archivo2->tipo, archItems);
    fputc(',', archItems);
    fputs(Archivo2->nombreI, archItems);
    fputc(',', archItems);
    fprintf(archItems, "%d", Archivo2->nivel_o_cantidad);
    fputc(',', archItems);
    fputs(Archivo2->nombreP, archItems);
    fputc('\n', archItems);
    Archivo2 = nextList(personaje);
    if (!Archivo2)break;    
  }
  printf("Archivo exportado con éxito.");
  fclose(archItems);
}