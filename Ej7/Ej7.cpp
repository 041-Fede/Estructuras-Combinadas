#include <iostream>
#include <stdio.h>

using namespace std;

struct EmpleadoDeArchivo
{
    int NroDeLegajo,Dni,CodDeDepartamentoActual;
    char Nombre[20];
};

struct VacanteDeArchivo
{
    int CodDeDepartamento,CantDeVacantes;
};

struct PostulanteDeArchivo
{
    int NroDeLegajo,CodDeDepartamento;
};

struct Empleado
{
    int NroDeLegajo;
};

struct NodoSub
{
    Empleado Info;
    NodoSub *Sgte;
};

struct Departamento
{
    int CodDeDepartamento;
    NodoSub *ListaDeLlegada;
};

struct Nodo
{
    Departamento Info;
    Nodo *Sgte;
};

void InicializarVector(int Vacantes[]);
void CargaDeVacantes(FILE *ArchivoB,int Vacantes[]);
void CargaDePostulantes(FILE *ArchivoC,Nodo *&ListaDePostulantes);
void InsertarEmpleado(NodoSub *&ListaDeLlegada,Empleado Dato);
void MostrarListado(FILE *ArchivoA,Nodo *ListaDePostulantes);
void Desencolar(NodoSub *&ListaDeLlegada);
void AsignacionDeDepartamento(FILE *ArchivoA,Nodo *ListaDePostulantes,int Vacantes[]);
int BusquedaSecuencial(FILE *ArchivoA,int NroDeLegajo,EmpleadoDeArchivo &E);
Nodo *BuscarInsertar(Nodo *&ListaDePostulantes,Departamento Dato);


int main()
{
    Nodo *ListaDePostulantes = NULL;
    FILE *ArchivoA = fopen("Empleados.dat","rb+");
    FILE *ArchivoB = fopen("Vacantes.dat","rb");
    FILE *ArchivoC = fopen("Postulantes.dat","rb");

    if(ArchivoA == NULL || ArchivoB == NULL || ArchivoC == NULL)
    {
        cout << "ERROR" << endl;
    }
    else
    {
        int Vacantes[50];

        InicializarVector(Vacantes);

        CargaDeVacantes(ArchivoB,Vacantes);

        CargaDePostulantes(ArchivoC,ListaDePostulantes);

        cout << "Listado ordenado por codigo de departamento con todos los empleados postulados al mismo por orden de llegada: " << endl;

        MostrarListado(ArchivoA,ListaDePostulantes);

        AsignacionDeDepartamento(ArchivoA,ListaDePostulantes,Vacantes);

    }

    return 0;
}

void InicializarVector(int Vacantes[])
{
    for(int i = 0; i < 50; i++)
    {
        Vacantes[i] = 0;
    }
}

void CargaDeVacantes(FILE *ArchivoB,int Vacantes[])
{
    VacanteDeArchivo V;

    fread(&V,sizeof(VacanteDeArchivo),1,ArchivoB);

    while(!feof(ArchivoB))
    {
        Vacantes[V.CodDeDepartamento - 1] = V.CantDeVacantes;

        fread(&V,sizeof(VacanteDeArchivo),1,ArchivoB);
    }
}

void CargaDePostulantes(FILE *ArchivoC,Nodo *&ListaDePostulantes)
{
    PostulanteDeArchivo P;
    Departamento D;
    Empleado E;
    Nodo *Aux;

    fread(&P,sizeof(PostulanteDeArchivo),1,ArchivoC);

    while(!feof(ArchivoC))
    {
        D.CodDeDepartamento = P.CodDeDepartamento;
        D.ListaDeLlegada = NULL;
        E.NroDeLegajo = P.NroDeLegajo;
        Aux = BuscarInsertar(ListaDePostulantes,D);

        InsertarEmpleado(Aux->Info.ListaDeLlegada,E);

        fread(&P,sizeof(PostulanteDeArchivo),1,ArchivoC);
    }

}

void InsertarEmpleado(NodoSub *&ListaDeLlegada,Empleado Dato)
{
    NodoSub *Aux;
    NodoSub *Nuevo = new NodoSub;
    Nuevo->Info = Dato;
    Nuevo->Sgte = NULL;

    if(ListaDeLlegada != NULL)
    {
        Aux = ListaDeLlegada;

        while(Aux->Sgte != NULL)
        {
            Aux = Aux->Sgte;
        }

        Aux->Sgte = Nuevo;
    }
    else
    {
        ListaDeLlegada = Nuevo;
    }
}

void MostrarListado(FILE *ArchivoA,Nodo *ListaDePostulantes)
{
    int i;
    EmpleadoDeArchivo E;
    Nodo *Aux;
    NodoSub *AuxSub;
    Aux = ListaDePostulantes;

    while(Aux != NULL)
    {
        cout << "Codigo de departamento: " << Aux->Info.CodDeDepartamento << endl;

        AuxSub = Aux->Info.ListaDeLlegada;

        while(AuxSub != NULL)
        {
            i = BusquedaSecuencial(ArchivoA,AuxSub->Info.NroDeLegajo,E);

            if(i != -1)
            {
                cout << "Nro de legajo: " << E.Nombre << " - " << "Dni: " << E.Dni << " - " << "Nombre: " << E.Nombre << endl;
            }

            fseek(ArchivoA,sizeof(EmpleadoDeArchivo),SEEK_SET);

            AuxSub = AuxSub->Sgte;
        }

        cout << "---------------------------------" << endl;

        Aux = Aux->Sgte;
    }
}

void AsignacionDeDepartamento(FILE *ArchivoA,Nodo *ListaDePostulantes,int Vacantes[])
{
    int Pos;
    EmpleadoDeArchivo E;
    Nodo *Aux;
    NodoSub *AuxSub;
    Aux = ListaDePostulantes;

    while(Aux != NULL)
    {
        AuxSub = Aux->Info.ListaDeLlegada;

        while(AuxSub != NULL)
        {
            Pos = BusquedaSecuencial(ArchivoA,AuxSub->Info.NroDeLegajo,E);

            if(Pos != -1)
            {
                if(Vacantes[Aux->Info.CodDeDepartamento - 1] > 0)
                {
                    Desencolar(AuxSub);

                    Vacantes[Aux->Info.CodDeDepartamento - 1] = Vacantes[Aux->Info.CodDeDepartamento - 1] - 1;

                    E.CodDeDepartamentoActual = Aux->Info.CodDeDepartamento;

                    fseek(ArchivoA,Pos*sizeof(EmpleadoDeArchivo),SEEK_SET);
                    fwrite(&E,sizeof(EmpleadoDeArchivo),1,ArchivoA);
                }
            }

            fseek(ArchivoA,sizeof(EmpleadoDeArchivo),SEEK_SET);
        }

        Aux = Aux->Sgte;
    }
}

void Desencolar(NodoSub *&ListaDeLlegada)
{
    NodoSub *Aux = ListaDeLlegada;
    ListaDeLlegada = Aux->Sgte;

    delete Aux;
}

int BusquedaSecuencial(FILE *ArchivoA,int NroDeLegajo,EmpleadoDeArchivo &E)
{
    int i = 0;

    fread(&E,sizeof(EmpleadoDeArchivo),1,ArchivoA);

    while(!feof(ArchivoA) && E.NroDeLegajo != NroDeLegajo)
    {
        i++;

        fread(&E,sizeof(EmpleadoDeArchivo),1,ArchivoA);
    }

    if(feof(ArchivoA))
    {
        return -1;
    }
    else
    {
        return i;
    }
}

Nodo *BuscarInsertar(Nodo *&ListaDePostulantes,Departamento Dato)
{
    Nodo *Aux,*Antecesor;
    Aux = ListaDePostulantes;

    while(Aux != NULL && Aux->Info.CodDeDepartamento < Dato.CodDeDepartamento)
    {
        Antecesor = Aux;
        Aux = Aux->Sgte;
    }

    if(Aux != NULL && Dato.CodDeDepartamento == Aux->Info.CodDeDepartamento)
    {
        return Aux;
    }
    else
    {
        Nodo *Nuevo = new Nodo;
        Nuevo->Info = Dato;
        Nuevo->Sgte = Aux;

        if(Aux != ListaDePostulantes)
        {
            Antecesor->Sgte = Nuevo;
        }
        else
        {
            ListaDePostulantes = Nuevo;
        }

        return Nuevo;
    }
}
