#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<string>
#include <math.h>
#include <string>

using namespace std;

int bin_dec(string n){
    int decimal = 0;
    int in = 0;
    for(int i=n.length()-1;i>=0;i--){
        if(n[i]=='1'){
            decimal += pow(2, in);
        }
        in++;
    }
    return decimal;
}

void binario_ver(char n)
{
    int i, sp,a,b;
    string cadena,cadena1,bin, num;
    sp=0;
    printf("--> Version: ");
    for(i=7;i>=0;i--)
    {
        bin=((n & (1 << i)) ? "1" : "0");
        if(i>=4)
        {
           cadena=cadena+bin;
        }
        else
        {
            cadena1=cadena1+bin;
        }
        sp++;
    }
    a=bin_dec(cadena);
    b=bin_dec(cadena1);
    cout<<a<<endl<<"--> Tamano: "<<b<<" palabras"<<endl;
}

string binario_8bits(char n)
{
    string resultado,bin;
    int a;
    for(int i=7;i>=0;i--){
        bin=((n & (1 << i)) ? "1" : "0");
        resultado=resultado+bin;
        }
    return resultado;
}

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

int protocolo(char n)
{
	int sp,a,b;

    string resultado,bin;
    for(int i=7;i>=0;i--){
        bin=((n & (1 << i)) ? "1" : "0");
        resultado=resultado+bin;
        }


	a=bin_dec(resultado);

	if(a==1){
		cout<<"ICMPv4"<<endl;
		return 1;
	}else if(a==6){
		cout<<"TCP"<<endl;
	}else if(a==17){
		cout<<"UDP"<<endl;
	}else if(a==58){
		cout<<"ICMPv6"<<endl;
	}else if(a==118){
		cout<<"STP"<<endl;
	}else if(a==121){
		cout<<"SMP"<<endl;
	}
}

int main()
{
    unsigned char(palabra);
    int i, tres=3;
    int opcionTipo, protocolo_op, ayuda_int=0;
    string ayuda, auxiliar, cero = "0", uno ="1";
    int version_r;

    FILE *archivo;

    if ((archivo = fopen("ethernet_ipv6_nd.bin","rb+")) == NULL)
        {
            printf (" Error en la apertura. Es posible que el fichero no exista. \n");
        }

    else
            {
                cout<<"                                     *** ETHERNET ***"<<endl<<endl;
                printf("--> Direccion MAC origen:\n");
                cout<<"    ";
                for(i=0;i<=5;i++){
                    palabra=fgetc(archivo);
                    printf ("%02x:",palabra);
                   }
                 printf("\n");


                printf("--> Direccion MAC destino:\n");
                cout<<"    ";
                for(i=0;i<=5;i++){
                    palabra=fgetc(archivo);
                    printf ("%02x:",palabra);
                }
                printf("\n");


                printf("--> Tipo de codigo:\n");
                cout<<"    ";
                for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    printf ("%02x:",palabra);
                }printf("\n");
                ayuda = binario_8bits(palabra);
                ayuda_int = bin_dec(ayuda);

                cout << "entro"<< ayuda_int;
                // switch con las opciones de los diferentes tipos
                switch(ayuda_int)
                {
                case 0:
                    printf("--> Tipo: IPv4. \n");
                    palabra=fgetc(archivo);
                    binario_ver(palabra);

                    palabra=fgetc(archivo);

                    //tipo de servicio
                    ayuda = binario_8bits(palabra);
                    //primeros 3 bits
                    for(i=0;i<=2;i++){
                        auxiliar+=ayuda[i];//guardamos 3 bits para ver el codigo y verificar de rutina
                    }
                    cout<<"--> Tipo de servicio: ";
                    if(auxiliar == "000"){
                        cout<<"De rutina"<<endl;
                    }
                    else if(auxiliar == "001")
                    {
                        cout<<"Prioritario."<<endl;
                    }
                    else if(auxiliar == "010")
                    {
                        cout<<"Inmediato."<<endl;
                    }
                    else if(auxiliar == "011")
                    {
                        cout<<"Relampago."<<endl;
                    }
                    else if(auxiliar == "100")
                    {
                        cout<<"Invalidacion Relampago."<<endl;
                    }
                    else if(auxiliar == "101")
                    {
                        cout<<"Procesando llamada cr�tica y de emergencia."<<endl;
                    }
                    else if(auxiliar == "001")
                    {
                        cout<<"Control de trabajo de Internet."<<endl;
                    }
                    else
                    {
                        cout<<"Control de red."<<endl;
                    }

                    auxiliar.clear();//limpiamos el string auxiliar
                    //cuarto bit
                    auxiliar = ayuda[3];
                    cout<<"     + Retardo: ";
                    if(auxiliar == "0"){
                        cout<< "normal"<< endl;
                    }
                    if(auxiliar == "1"){
                        cout<<"bajo"<<endl;
                    }

                    //quinto bit
                    auxiliar.clear();//limpiamos el string auxiliar
                    auxiliar = ayuda[4];
                    cout<<"     + Rendimiento: ";
                    if(auxiliar == "0"){
                        cout<< "bajo"<< endl;
                    }
                    if(auxiliar == "1"){
                        cout<<"alto"<<endl;
                    }

                    //sexto bit
                    auxiliar.clear();//limpiamos el string auxiliar
                    auxiliar = ayuda[5];
                    cout<<"     + Fiabilidad: ";
                    if(auxiliar == "0"){
                        cout<< "normal"<< endl;
                    }
                    if(auxiliar == "1"){
                        cout<<"alta"<<endl;
                    }
                    auxiliar.clear();
                    ayuda.clear();

                    //longitud total
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    auxiliar += binario_8bits(palabra);
                    }
                    //cout<<auxiliar<<endl;
                    cout <<"--> Longitud total: " <<bin_dec(auxiliar)<<" octetos "<<endl;
                    auxiliar.clear();

                    //identificador
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    auxiliar += binario_8bits(palabra);
                    }
                    //cout<<auxiliar<<endl;
                    cout <<"--> Identificador: " <<bin_dec(auxiliar)<<endl;
                    auxiliar.clear();

                    //flags
                    cout<<"--> Flags(Banderas): "<<endl;
                    ayuda = auxiliar[0];
                    if(ayuda=="0"){
                        cout<<"     + Reservado"<<endl;
                    }
                    if(ayuda=="1"){
                        cout<<"     + Imposible"<<endl;
                    }
                    ayuda = auxiliar[1];
                    if(ayuda=="0"){
                        cout<<"     + Divisible"<<endl;
                    }
                    if(ayuda=="1"){
                        cout<<"     + No divisible"<<endl;
                    }
                    ayuda = auxiliar[2];
                    if(ayuda=="0"){
                        cout<<"     + Ultimo fragmento"<<endl;
                    }
                    if(ayuda=="1"){
                        cout<<"     + Fragmento intermedio"<<endl;
                    }
                    //posicion de fragmento
                    ayuda.clear();
                    for(i=3;i<=17;i++){
                        ayuda+=auxiliar[i];
                    }
                    cout<<"--> Posicion de fragmento: "<<bin_dec(ayuda)<<endl;
                    ayuda.clear();
                    auxiliar.clear();

                    //tiempo de vida
                    palabra= fgetc(archivo);
                    auxiliar= binario_8bits(palabra);
                    cout<<"--> Tiempo de vida es: "<<bin_dec(auxiliar)<<endl;;
                    ayuda.clear();
                    auxiliar.clear();

                    //protcolo
                    cout<<"--> Protocolo: ";
                    for(i=0;i<=2;i++){
                        palabra=fgetc(archivo);
                    }
                    version_r = protocolo(palabra);
                    ayuda.clear();
                    auxiliar.clear();


                    //checksum
                    cout<<"--> Checksum:"<<endl;
                    cout<<"    ";
                    for(i=0;i<=1;i++){
                        palabra=fgetc(archivo);
                        printf ("%02x:",palabra);

                    }
                    cout << endl;

                    //direccion ip origen
                    cout<<"--> Direccion IP de origen: "<<endl;
                    cout<<"    ";
                    for(i=0;i<=3;i++){

                        palabra=fgetc(archivo);
                        auxiliar = binario_8bits(palabra);
                        cout<<bin_dec(auxiliar)<<".";
                    }
                    cout<<endl;
                    cout<<"--> Direccion IP de destino: "<<endl;
                    cout<<"    ";
                    for(i=0;i<=3;i++){
                        palabra=fgetc(archivo);
                        auxiliar = binario_8bits(palabra);
                        cout<<bin_dec(auxiliar)<<".";
                    }
                    cout<<endl<<endl;
                    auxiliar.clear();
                    ayuda.clear();
                    if(version_r==1){
                        int ayuda_2=0;

                        palabra=fgetc(archivo);

                        auxiliar.clear();
                        auxiliar = binario_8bits(palabra);

                        ayuda_2 = bin_dec(auxiliar);

                        cout<<"--> Tipo de mensaje informativo:"<<endl;

                        //ifs
                        if(ayuda_2==0){
                            cout<<"     + Echo Reply (respuesta de eco)"<<endl;
                        }
                        if(ayuda_2==3){
                            cout<<"     + Destination Unreacheable (destino inaccesible)"<<endl;
                        }
                        if(ayuda_2==4){
                            cout<<"     + Source Quench (disminución del tráfico desde el origen)"<<endl;
                        }
                        if(ayuda_2==5){
                            cout<<"     + Redirect (redireccionar - cambio de ruta) "<<endl;
                        }
                        if(ayuda_2==8){
                            cout<<"     + Echo (solicitud de eco)"<<endl;
                        }
                        if(ayuda_2==11){
                            cout<<"     + Time Exceeded (tiempo excedido para un datagrama)"<<endl;
                        }
                        if(ayuda_2==12){
                            cout<<"     + Parameter Problem (problema de parámetros"<<endl;
                        }
                        if(ayuda_2==13){
                            cout<<"     + Timestamp (solicitud de marca de tiempo)"<<endl;
                        }
                        if(ayuda_2==14){
                            cout<<"     + Timestamp Reply (respuesta de marca de tiempo)"<<endl;
                        }
                        if(ayuda_2==15){
                            cout<<"     + Time Exceeded (tiempo excedido para un datagrama)"<<endl;
                        }
                        if(ayuda_2==16){
                            cout<<"     + Information Request (solicitud de información) - obsoleto-"<<endl;
                        }
                        if(ayuda_2==17){
                            cout<<"     + Addressmask (solicitud de máscara de dirección)"<<endl;
                        }
                        if(ayuda_2==18){
                            cout<<"     + Addressmask Reply (respuesta de máscara de dirección"<<endl;
                        }
                        palabra=fgetc(archivo);
                        auxiliar.clear();
                        auxiliar = binario_8bits(palabra);

                        ayuda_2 = bin_dec(auxiliar);

                        cout<<"--> Codigo de error: "<<endl;

                        if(ayuda_2==0){
                            cout<<"     + No se puede llegar a la red"<<endl;
                        }
                        if(ayuda_2==1){
                            cout<<"     + No se puede llegar al host o aplicación de destino"<<endl;
                        }
                        if(ayuda_2==2){
                            cout<<"     + El destino no dispone del protocolo solicitado"<<endl;
                        }
                        if(ayuda_2==3){
                            cout<<"     + No se puede llegar al puerto destino o la aplicacion destino no esta libre "<<endl;
                        }
                        if(ayuda_2==4){
                            cout<<"     + Se necesita aplicar fragmentacion, pero el flag correspondiente indica lo contrario"<<endl;
                        }
                        if(ayuda_2==5){
                            cout<<"     + La ruta de origen no es correcta "<<endl;
                        }
                        if(ayuda_2==6){
                            cout<<"     + No se conoce la red destino "<<endl;
                        }
                        if(ayuda_2==7){
                            cout<<"     + No se conoce el host destino "<<endl;
                        }
                        if(ayuda_2==8){
                            cout<<"     + El host origen esta aislado "<<endl;
                        }
                        if(ayuda_2==9){
                            cout<<"     + La comunicacion con la red destino esta prohibida por razones administrativas"<<endl;
                        }

                        cout<<"--> Checksum:"<<endl;
                        cout<<"    ";
                        for(i=0;i<=1;i++){
                            palabra=fgetc(archivo);
                            printf ("%02x:",palabra);

                    }
                    cout << endl<<endl;
                    }
                    break;
                case 6:
                    printf("Tipo: ARP. \n");
                    auxiliar.clear();
                    ayuda.clear();
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    auxiliar += binario_8bits(palabra);
                    }
                   // cout<<bin_dec(auxiliar);
                    ayuda_int=bin_dec(auxiliar);
                    if(ayuda_int==1){
                        cout<<"Ethernet (10 Mb)"<<endl;
                    }
                    auxiliar.clear();
                    ayuda.clear();
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    
                    }

                    //agregar los demas switches
                    switch(palabra){
                        case 00:
                            cout<<"IPv4"<<endl;
                    }
                    //logintud direccion hardware
                    palabra=fgetc(archivo);
                    cout<<"longitud de la direccion hardware:";
                    auxiliar.clear();
                    ayuda.clear();
                    ayuda = binario_8bits(palabra);
                    ayuda_int = bin_dec(ayuda);
                    cout<<" "<<ayuda_int<<endl;
                    

                    //longitud protocolo
                    palabra=fgetc(archivo);
                    cout<<"longitud de la direccion de protocolo:";
                    auxiliar.clear();
                    ayuda.clear();
                    ayuda = binario_8bits(palabra);
                    ayuda_int = bin_dec(ayuda);
                    cout<<" "<<ayuda_int<<endl;

                //codigo de operacion
                    auxiliar.clear();
                    ayuda.clear();
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    auxiliar+=binario_8bits(palabra);
                    }              
                    ayuda_int=bin_dec(auxiliar);
                    if(ayuda_int==1){
                        cout<<"solicitud ARP"<<endl;
                    }
                    if(ayuda_int==2){
                        cout<<"respuesta ARP"<<endl;
                    }
                    if(ayuda_int==3){
                        cout<<"solicitud RARP"<<endl;
                    }
                    if(ayuda_int==4){
                        cout<<"respuesta RARP"<<endl;
                    }

                    auxiliar.clear();
                    ayuda.clear();
                    cout<<"direccion de hardware del emisor (MAC)"<<endl;
                    for(i=0;i<=5;i++){
                        palabra = getc(archivo);
                        printf("%02x:",palabra);
                    }
                    cout<<endl;

                    cout<< "Direccion IP de emisor"<<endl;
                    for(i=0;i<=3;i++){
                        palabra = getc(archivo);
                        auxiliar = binario_8bits(palabra);
                        ayuda_int = bin_dec(auxiliar);
                        cout<<ayuda_int<<".";
                    }
                    cout<<endl;
                    auxiliar.clear();
                    ayuda.clear();
                    cout<<"direccion de hardware del receptor (MAC)"<<endl;
                    for(i=0;i<=5;i++){
                        palabra = getc(archivo);
                        printf("%02x:",palabra);
                    }
                    cout<<endl;

                    cout<< "Direccion IP de receptor"<<endl;
                    for(i=0;i<=3;i++){
                        palabra = getc(archivo);
                        auxiliar = binario_8bits(palabra);
                        ayuda_int = bin_dec(auxiliar);
                        cout<<ayuda_int<<".";
                    }
                    cout<<endl;

                    break;
                case 35:
                    printf("Tipo: RARP. \n");
                    auxiliar.clear();
                    ayuda.clear();
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    auxiliar += binario_8bits(palabra);
                    }
                    cout<<bin_dec(auxiliar);
                    ayuda_int=bin_dec(auxiliar);
                    if(ayuda_int==1){
                        cout<<"Ethernet (10 Mb)"<<endl;
                    }
                    auxiliar.clear();
                    ayuda.clear();
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    
                    }

                    //agregar los demas switches
                    switch(palabra){
                        case 00:
                            cout<<"IPv4"<<endl;
                    }
                    //logintud direccion hardware
                    palabra=fgetc(archivo);
                    cout<<"longitud de la direccion hardware:";
                    auxiliar.clear();
                    ayuda.clear();
                    ayuda = binario_8bits(palabra);
                    ayuda_int = bin_dec(ayuda);
                    cout<<" "<<ayuda_int<<endl;
                    

                    //longitud protocolo
                    palabra=fgetc(archivo);
                    cout<<"longitud de la direccion de protocolo:";
                    auxiliar.clear();
                    ayuda.clear();
                    ayuda = binario_8bits(palabra);
                    ayuda_int = bin_dec(ayuda);
                    cout<<" "<<ayuda_int<<endl;

                //codigo de operacion
                    auxiliar.clear();
                    ayuda.clear();
                    for(i=0;i<=1;i++){
                    palabra=fgetc(archivo);
                    auxiliar+=binario_8bits(palabra);
                    }              
                    ayuda_int=bin_dec(auxiliar);
                    if(ayuda_int==1){
                        cout<<"solicitud ARP"<<endl;
                    }
                    if(ayuda_int==2){
                        cout<<"respuesta ARP"<<endl;
                    }
                    if(ayuda_int==3){
                        cout<<"solicitud RARP"<<endl;
                    }
                    if(ayuda_int==4){
                        cout<<"respuesta RARP"<<endl;
                    }

                    auxiliar.clear();
                    ayuda.clear();
                    cout<<"direccion de hardware del emisor (MAC)"<<endl;
                    for(i=0;i<=5;i++){
                        palabra = getc(archivo);
                        printf("%02x:",palabra);
                    }
                    cout<<endl;

                    cout<< "Direccion IP de emisor"<<endl;
                    for(i=0;i<=3;i++){
                        palabra = getc(archivo);
                        auxiliar = binario_8bits(palabra);
                        ayuda_int = bin_dec(auxiliar);
                        cout<<ayuda_int<<".";
                    }
                    cout<<endl;
                    auxiliar.clear();
                    ayuda.clear();
                    cout<<"direccion de hardware del receptor (MAC)"<<endl;
                    for(i=0;i<=5;i++){
                        palabra = getc(archivo);
                        printf("%02x:",palabra);
                    }
                    cout<<endl;

                    cout<< "Direccion IP de receptor"<<endl;
                    for(i=0;i<=3;i++){
                        palabra = getc(archivo);
                        auxiliar = binario_8bits(palabra);
                        ayuda_int = bin_dec(auxiliar);
                        cout<<ayuda_int<<".";
                    }
                    cout<<endl;
                    
                    
                    break;
                case 221:
                    printf("Tipo: IPv6. \n");


                    //version

                    ayuda.clear();
                    auxiliar.clear();
                    palabra = getc(archivo);
                    ayuda = binario_8bits(palabra);
                    for(i=0;i<=3;i++){
                        auxiliar += ayuda[i]; 
                    }
                    ayuda_int = bin_dec(auxiliar);
                    if(ayuda_int==6){
                        cout<<"version 6"<<endl;
                    }
                    else{
                        cout << "error"<<endl;
                    }
                    auxiliar.clear();
                    for (i = 4; i <= 7; i++){
                        auxiliar += ayuda[i];
                    }
                    ayuda.clear();
                    //clase de trafico

                    palabra = getc(archivo);
                    ayuda = binario_8bits(palabra);
                    for(i=0;i<=3;i++){
                        auxiliar+=ayuda[i];
                    }
                    string auxiliar_apoyo;

                    for(i=0;i<=2;i++){
                        auxiliar_apoyo += auxiliar[i];
                    }
                    cout<<"Clase de trafico: "<<auxiliar<<endl;

                    cout<<"--> Tipo de servicio: ";
                    if(auxiliar_apoyo == "000"){
                        cout<<"De rutina"<<endl;
                    }
                    else if(auxiliar_apoyo == "001")
                    {
                        cout<<"Prioritario."<<endl;
                    }
                    else if(auxiliar_apoyo == "010")
                    {
                        cout<<"Inmediato."<<endl;
                    }
                    else if(auxiliar_apoyo == "011")
                    {
                        cout<<"Relampago."<<endl;
                    }
                    else if(auxiliar_apoyo == "100")
                    {
                        cout<<"Invalidacion Relampago."<<endl;
                    }
                    else if(auxiliar_apoyo == "101")
                    {
                        cout<<"Procesando llamada cr�tica y de emergencia."<<endl;
                    }
                    else if(auxiliar_apoyo == "001")
                    {
                        cout<<"Control de trabajo de Internet."<<endl;
                    }
                    else
                    {
                        cout<<"Control de red."<<endl;
                    }

                    auxiliar_apoyo.clear();//limpiamos el string auxiliar
                    //cuarto bit
                    auxiliar_apoyo = auxiliar[3];
                    cout<<"     + Retardo: ";
                    if(auxiliar_apoyo == "0"){
                        cout<< "normal"<< endl;
                    }
                    if(auxiliar_apoyo == "1"){
                        cout<<"bajo"<<endl;
                    }

                    //quinto bit
                    auxiliar_apoyo.clear();//limpiamos el string auxiliar
                    auxiliar_apoyo = auxiliar[4];
                    cout<<"     + Rendimiento: ";
                    if(auxiliar_apoyo == "0"){
                        cout<< "bajo"<< endl;
                    }
                    if(auxiliar_apoyo == "1"){
                        cout<<"alto"<<endl;
                    }

                    //sexto bit
                    auxiliar_apoyo.clear();//limpiamos el string auxiliar
                    auxiliar_apoyo = auxiliar[5];
                    cout<<"     + Fiabilidad: ";
                    if(auxiliar_apoyo == "0"){
                        cout<< "normal"<< endl;
                    }
                    if(auxiliar_apoyo == "1"){
                        cout<<"alta"<<endl;
                    }
                    auxiliar_apoyo.clear();
                    
                    auxiliar.clear();
                    //etiqueta de flujo

                    for(i=4;i<=7;i++){
                        auxiliar += ayuda[i];
                    }

                    palabra = getc(archivo);
                    ayuda.clear();
                    ayuda= binario_8bits(palabra);
                    auxiliar += ayuda;

                    palabra = getc(archivo);
                    ayuda.clear();
                    ayuda= binario_8bits(palabra);
                    auxiliar += ayuda;

                    cout<<"Etiqueta de flujo: "<<bin_dec(auxiliar)<<endl;
                    
                    //tamaño de datos
                    ayuda.clear();
                    auxiliar.clear();
                    auxiliar_apoyo.clear();

                    for(i=0;i<=1;i++){
                        palabra = fgetc(archivo);
                        auxiliar += binario_8bits(palabra);
                    }
                    cout<<"Tamaño de datos: "<<bin_dec(auxiliar)<<endl;

                    //encabezado siguiente
                    ayuda.clear();
                    auxiliar.clear();
                    auxiliar_apoyo.clear();

                    palabra = getc(archivo);
                    auxiliar = binario_8bits(palabra);
                    cout<<"Encabezado siguiente: "<<bin_dec(auxiliar)<<endl;

                    // LIMITE DE SALTO
                    ayuda.clear();
                    auxiliar.clear();
                    auxiliar_apoyo.clear();

                    palabra = getc(archivo);
                    auxiliar = binario_8bits(palabra);
                    cout<<"limite de salto: "<<bin_dec(auxiliar)<<endl;

                    //direccion de origen
                    cout<<"direccion de origen: ";
                    for(i=0; i<=15;i++){
                        palabra = getc(archivo);
                        printf("%x:",palabra);
                    }
                    cout<<endl;
                   cout<<"direccion de destino: ";
                    for(i=0; i<=15;i++){
                        palabra = getc(archivo);
                        printf("%x:",palabra);
                    }
                    cout<<endl;
                    break;
                }

                printf("--> Datos: ");
                printf("\n    ");
                while (!feof(archivo)){
                    palabra=fgetc(archivo);
                    printf ("%02x:",palabra);
                     }
               printf("\n");
        }

        fclose(archivo);
        return (0);
}
