#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<string>
#include <math.h>
#include <string>
#include <ctype.h>
#include <fstream>
#include <pcap.h>
#include <ostream>
#define LINE_LEN 16

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
unsigned int bin_dec_double(string n){
    unsigned int decimal = 0;
    unsigned int in = 0;
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

int crear_doc(int argc, char **argv){

	pcap_if_t *alldevs, *d;
	pcap_t *fp;
	u_int inum, i=0;
	char errbuf[PCAP_ERRBUF_SIZE];
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	fstream lectura;


	printf("pktdump_ex: prints the packets of the network using WinPcap.\n");
	printf("   Usage: pktdump_ex [-s source]\n\n"
		"   Examples:\n"
		"      pktdump_ex -s file.acp\n"
		"      pktdump_ex -s \\Device\\NPF_{C8736017-F3C3-4373-94AC-9A34B7DAD998}\n\n");

	if(argc < 3)
	{
		printf("\nNo adapter selected: printing the device list:\n");
		/* The user didn't provide a packet source: Retrieve the local device list */
		if(pcap_findalldevs(&alldevs, errbuf) == -1)
		{
			fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
			exit(1);
		}

		/* Print the list */
		for(d=alldevs; d; d=d->next)
		{
			printf("%d. %s\n    ", ++i, d->name);

			if (d->description)
				printf(" (%s)\n", d->description);
			else
				printf(" (No description available)\n");
		}

		if (i==0)
		{
			printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
			return -1;
		}

		printf("Enter the interface number (1-%d):",i);
		scanf("%d", &inum);

		if (inum < 1 || inum > i)
		{
			printf("\nInterface number out of range.\n");

			/* Free the device list */
			pcap_freealldevs(alldevs);
			return -1;
		}

		/* Jump to the selected adapter */
		for (d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

		/* Open the adapter */
		if ((fp = pcap_open_live(d->name,	// name of the device
			65536,							// portion of the packet to capture.
											// 65536 grants that the whole packet will be captured on all the MACs.
			1,								// promiscuous mode (nonzero means promiscuous)
			1000,							// read timeout
			errbuf							// error buffer
			)) == NULL)
		{
			fprintf(stderr,"\nError opening adapter\n");
			return -1;
		}
	}
	else
	{
		/* Do not check for the switch type ('-s') */
		if ((fp = pcap_open_live(argv[2],	// name of the device
			65536,							// portion of the packet to capture.
											// 65536 grants that the whole packet will be captured on all the MACs.
			1,								// promiscuous mode (nonzero means promiscuous)
			1000,							// read timeout
			errbuf							// error buffer
			)) == NULL)
		{
			fprintf(stderr,"\nError opening adapter\n");
			return -1;
		}
	}

	/* Read the packets */
	lectura.open("datos.bin",ios::out);
	string caracter_archivo;
	int bandera_tronar, flag_tronar=0;


	while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0)
	{


        if(flag_tronar == 1){
            res = -1;
            break;
        }
        flag_tronar+=1;
		if(res == 0)
			/* Timeout elapsed */
			continue;

		/* print pkt timestamp and pkt len */
		printf("%ld:%ld (%ld)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);

		/* Print the packet */



		for (i=1; (i < header->caplen + 1 ) ; i++)
		{
			printf("%.2x ", pkt_data[i-1]);

            caracter_archivo = pkt_data[i-1];


			lectura << caracter_archivo;
            caracter_archivo.clear();



			if ( (i % LINE_LEN) == 0) printf("\n");
		}

		printf("\n\n");
	}
    lectura.close();
	if(res == -1)
	{
		printf("Error reading the packets: %s\n", pcap_geterr(fp));
		return -1;
	}

	pcap_close(fp);
    return bandera_tronar;
}


int main(int argc, char **argv)
{

    int ciclos_lectura;





	//desde aqui comienza mi programa


    unsigned char(palabra);
    int tres=3,i;
    int opcionTipo, protocolo_op, ayuda_int=0, bandera_lectura=0;

    string ayuda, auxiliar, cero = "0", uno ="1";
    int version_r;
    int bandera_DNS = 0;

    FILE *archivo;
    cout<<"Dame el numero de ciclos de lectura de paquetes que quieres"<<endl;
    cin>>ciclos_lectura;

    while(bandera_lectura<=ciclos_lectura-1){
        bandera_lectura += 1;
        crear_doc(argc, argv);
    if ((archivo = fopen("datos.bin","rb+")) == NULL)
        {
            printf (" Error en la apertura. Es posible que el fichero no exista. \n");
        }
    else
        {



            cout<<"                                     *** ETHERNET ***"<<endl<<endl;
            printf("--> Direccion MAC origen:\n");
            cout<<"    ";
            for(i=0;i<=5;i++)
                {
                    palabra=fgetc(archivo);
                    printf ("%02x:",palabra);
                }
    printf("\n");

    printf("--> Direccion MAC destino:\n");
    cout<<"    ";
    for(i=0;i<=5;i++)
        {
            palabra=fgetc(archivo);
            printf ("%02x:",palabra);
        }
    printf("\n");


    printf("--> Tipo de codigo:\n");
    cout<<"    ";
    for(i=0;i<=1;i++)
        {
        palabra=fgetc(archivo);
        printf ("%02x:",palabra);
        }
    printf("\n");
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
            for(i=0;i<=2;i++)
                {
                auxiliar+=ayuda[i];//guardamos 3 bits para ver el codigo y verificar de rutina
                }
            cout<<"--> Tipo de servicio: ";
            if(auxiliar == "000")
                {
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
                cout<<"Procesando llamada cr???tica y de emergencia."<<endl;
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
            if(auxiliar == "0")
                {
                cout<< "normal"<< endl;
                }
            if(auxiliar == "1")
                {
                cout<<"bajo"<<endl;
                }

            //quinto bit
            auxiliar.clear();//limpiamos el string auxiliar
            auxiliar = ayuda[4];
            cout<<"     + Rendimiento: ";
            if(auxiliar == "0")
                {
                cout<< "bajo"<< endl;
                }
            if(auxiliar == "1")
                {
                cout<<"alto"<<endl;
                }

            //sexto bit
            auxiliar.clear();//limpiamos el string auxiliar
            auxiliar = ayuda[5];
            cout<<"     + Fiabilidad: ";
            if(auxiliar == "0")
                {
                cout<< "normal"<< endl;
                }
            if(auxiliar == "1")
                {
                cout<<"alta"<<endl;
                }
            auxiliar.clear();
            ayuda.clear();

            //longitud total
            for(i=0;i<=1;i++)
                {
                palabra=fgetc(archivo);
                auxiliar += binario_8bits(palabra);
                }
            //cout<<auxiliar<<endl;
            cout <<"--> Longitud total: " <<bin_dec(auxiliar)<<" octetos "<<endl;
            auxiliar.clear();

            //identificador
            for(i=0;i<=1;i++)
                {
                palabra=fgetc(archivo);
                auxiliar += binario_8bits(palabra);
                }
            //cout<<auxiliar<<endl;
            cout <<"--> Identificador: " <<bin_dec(auxiliar)<<endl;
            auxiliar.clear();

            //flags
            cout<<"--> Flags(Banderas): "<<endl;
            ayuda = auxiliar[0];
            if(ayuda=="0")
                {
                cout<<"     + Reservado"<<endl;
                }
            if(ayuda=="1")
                {
                cout<<"     + Imposible"<<endl;
                }
            ayuda = auxiliar[1];
            if(ayuda=="0")
                {
                cout<<"     + Divisible"<<endl;
                }
            if(ayuda=="1")
                {
                cout<<"     + No divisible"<<endl;
                }
            ayuda = auxiliar[2];
            if(ayuda=="0")
                {
                cout<<"     + Ultimo fragmento"<<endl;
                }
            if(ayuda=="1")
                {
                cout<<"     + Fragmento intermedio"<<endl;
                }
            //posicion de fragmento
            ayuda.clear();
            for(i=3;i<=17;i++)
                {
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
            for(i=0;i<=2;i++)
                {
                palabra=fgetc(archivo);
                }
            version_r = protocolo(palabra);
            version_r = bin_dec(binario_8bits(palabra));

            ayuda.clear();
            auxiliar.clear();


            //checksum
            cout<<"--> Checksum:"<<endl;
            cout<<"    ";
            for(i=0;i<=1;i++)
                {
                palabra=fgetc(archivo);
                printf ("%02x:",palabra);
                }
            cout << endl;

            //direccion ip origen
            cout<<"--> Direccion IP de origen: "<<endl;
            cout<<"    ";
            for(i=0;i<=3;i++)
                {

                palabra=fgetc(archivo);
                auxiliar = binario_8bits(palabra);
                cout<<bin_dec(auxiliar)<<".";
                }
            cout<<endl;
            cout<<"--> Direccion IP de destino: "<<endl;
            cout<<"    ";
            for(i=0;i<=3;i++)
                {
                palabra=fgetc(archivo);
                auxiliar = binario_8bits(palabra);
                cout<<bin_dec(auxiliar)<<".";
                }
            cout<<endl<<endl;
            auxiliar.clear();
            ayuda.clear();
                    //ICMPv4
            if(version_r==1)
            {
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
                    cout<<"     + Source Quench (disminuci??n del tr??fico desde el origen)"<<endl;
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
                    cout<<"     + Parameter Problem (problema de par??metros"<<endl;
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
                    cout<<"     + Information Request (solicitud de informaci??n) - obsoleto-"<<endl;
                }
                if(ayuda_2==17){
                    cout<<"     + Addressmask (solicitud de m??scara de direcci??n)"<<endl;
                }
                if(ayuda_2==18){
                    cout<<"     + Addressmask Reply (respuesta de m??scara de direcci??n"<<endl;
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
                    cout<<"     + No se puede llegar al host o aplicaci??n de destino"<<endl;
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

                    //tcp
                    cout << endl<<endl;
                    }
                    //tcp
            if(version_r==6){
                        cout<<"TCP"<<endl;
                        //tcp
                        ayuda.clear();
                        auxiliar.clear();
                        //puerto de origen
                        for(i=0;i<=1;i++){
                            palabra = getc(archivo);
                            ayuda += binario_8bits(palabra);
                        }

                        ayuda_int= bin_dec(ayuda);
                            cout<< "Puerto de origen: "<<ayuda_int<<endl;
                            cout<< "Puerto de origen catalogado: ";
                            if(ayuda_int>=0 and ayuda_int<= 1023){
                                cout<<"Puertos bien conocidos"<<endl;
                                if(ayuda_int==20){
                                cout<<"     +Servicio: FTP"<<endl;
                                cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_int==21){
                                cout<<"     +Servicio: FTP"<<endl;
                                cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_int==22){
                                cout<<"     +Servicio: SSH"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==23){
                                cout<<"     +Servicio: TELNET"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==25){
                                cout<<"     +Servicio: SMTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==53){
                                cout<<"     +Servicio: DNC"<<endl;
                                cout<<"     +Protocolo:  TCP/UDP"<<endl;
                            }
                            if(ayuda_int==67){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==68){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==69){
                                cout<<"     +Servicio: TFTP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==80){
                                cout<<"     +Servicio: HTTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==110){
                                cout<<"     +Servicio: POP3"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==143){
                                cout<<"     +Servicio: IMAP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==443){
                                cout<<"     +Servicio: HTTPS"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==993){
                                cout<<"     +Servicio: IMAP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==995){
                                cout<<"     +Servicio: POP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                        }
                        if(ayuda_int>=1024 and ayuda_int<= 49151){
                            cout<<"Puertos registrados"<<endl;
                        }
                        if(ayuda_int>=49152 and ayuda_int<=65535){
                            cout<<"Puertos dinamicos o privados"<<endl;
                        }
                        ayuda.clear();
                        //puerto de destino
                        for(i=0;i<=1;i++){
                            palabra = getc(archivo);
                            ayuda += binario_8bits(palabra);
                        }

                        ayuda_int= bin_dec(ayuda);
                        cout<< "Puerto de destino: "<<ayuda_int<<endl;
                        cout<< "Puerto de destino catalogado: ";
                        if(ayuda_int>=0 and ayuda_int<= 1023){
                            cout<<"Puertos bien conocidos"<<endl;
                            if(ayuda_int==20){
                                cout<<"     +Servicio: FTP"<<endl;
                                cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_int==21){
                                cout<<"     +Servicio: FTP"<<endl;
                                cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_int==22){
                                cout<<"     +Servicio: SSH"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==23){
                                cout<<"     +Servicio: TELNET"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==25){
                                cout<<"     +Servicio: SMTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==53){
                                cout<<"     +Servicio: DNC"<<endl;
                                cout<<"     +Protocolo:  TCP/UDP"<<endl;
                            }
                            if(ayuda_int==67){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==68){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==69){
                                cout<<"     +Servicio: TFTP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==80){
                                cout<<"     +Servicio: HTTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==110){
                                cout<<"     +Servicio: POP3"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==143){
                                cout<<"     +Servicio: IMAP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==443){
                                cout<<"     +Servicio: HTTPS"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==993){
                                cout<<"     +Servicio: IMAP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==995){
                                cout<<"     +Servicio: POP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                        }
                        if(ayuda_int>=1024 and ayuda_int<= 49151){
                            cout<<"Puertos registrados"<<endl;
                        }
                        if(ayuda_int>=49152 and ayuda_int<=65535){
                            cout<<"Puertos dinamicos o privados"<<endl;
                        }
                        //Numero de secuencias
                        cout<<"numeros de secuencias"<<endl;
                        ayuda.clear();
                        auxiliar.clear();
                        for(i=0;i<=3;i++){
                            palabra = getc(archivo);
                            ayuda += binario_8bits(palabra);
                        }
                        cout<< "Numero de secuencia es: "<<bin_dec_double(ayuda)<<endl;

                        //Numero de acuse de recibo
                        ayuda.clear();
                        auxiliar.clear();
                        for(i=0;i<=3;i++){
                            palabra = getc(archivo);
                            ayuda += binario_8bits(palabra);
                        }
                        cout<< "Numero de acuse de recibo: "<<bin_dec_double(ayuda)<<endl;
                            ayuda.clear();
                        palabra = getc(archivo);
                        auxiliar = binario_8bits(palabra);
                        for(i=0; i<=3;i++){
                            ayuda += auxiliar[i];
                        }
                        ayuda_int = bin_dec(ayuda);
                        cout << "Longitud de cabecera: "<< ayuda_int << endl;
                        cout << "------------BANDERAS DE COMUNICACION TCP--------------"<<endl;
                        ayuda.clear();
                        ayuda = auxiliar[7];
                        cout<< "NS: ";
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        ayuda.clear();
                        auxiliar.clear();
                        palabra = getc(archivo);
                        auxiliar = binario_8bits(palabra);
                        cout << "CWR: ";
                        ayuda = auxiliar[0];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        cout<<"ECE: ";
                        ayuda.clear();
                        ayuda = auxiliar[1];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        cout<< "URG: ";
                        ayuda.clear();
                        ayuda = auxiliar[2];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        cout<<"ACK: ";
                        ayuda.clear();
                        ayuda = auxiliar[3];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        cout<<"PSH: ";
                        ayuda.clear();
                        ayuda = auxiliar[4];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        cout<<"RST: ";
                        ayuda.clear();
                        ayuda = auxiliar[5];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        cout<<"SYN: ";
                        ayuda.clear();
                        ayuda = auxiliar[6];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                        cout<<"FIN: ";
                        ayuda.clear();
                        ayuda = auxiliar[7];
                        if(ayuda == "0"){
                            cout<< "desactivado"<<endl;
                        }
                        if(ayuda == "1"){
                            cout << "activado"<<endl;
                        }
                    ayuda.clear();
                    auxiliar.clear();
                    cout<< "---------TAMA??O DE VENTANA O VENTANA DE RECEPCION----------"<<endl;
                    for(i=0;i<=1;i++){
                        palabra = fgetc(archivo);
                        auxiliar = binario_8bits(palabra);
                    }
                    cout << "Tama??o de ventana o ventana de recepcion: "<< bin_dec(auxiliar)<<endl;
                    auxiliar.clear();
                    cout<<"-----------checksum------------"<<endl;
                    for(i=0;i<=1;i++){
                        palabra = fgetc(archivo);
                        printf("%x:",palabra);
                    }
                    cout<<endl;
                    cout<< "----------Puntero urgente----------"<<endl;
                    for(i=0;i<=1;i++){
                        palabra = fgetc(archivo);
                        auxiliar = binario_8bits(palabra);
                    }
                    cout << "Puntero urgente: "<< bin_dec(auxiliar)<<endl;
                    }
                    //udp
            if(version_r==17){

                        cout<<"UDP"<<endl;
                        //UDP
                        ayuda.clear();
                        auxiliar.clear();
                        //puerto de origen
                        for(i=0;i<=1;i++){
                            palabra = getc(archivo);
                            ayuda += binario_8bits(palabra);
                        }

                        ayuda_int= bin_dec(ayuda);
                        cout<< "Puerto de origen: "<<ayuda_int<<endl;
                        cout<< "Puerto de origen catalogado: ";
                        if(ayuda_int>=0 and ayuda_int<= 1023){
                            cout<<"Puertos bien conocidos"<<endl;
                            if(ayuda_int==20){
                            cout<<"     +Servicio: FTP"<<endl;
                            cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_int==21){
                                cout<<"     +Servicio: FTP"<<endl;
                                cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_int==22){
                                cout<<"     +Servicio: SSH"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==23){
                                cout<<"     +Servicio: TELNET"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==25){
                                cout<<"     +Servicio: SMTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==53){
                                cout<<"     +Servicio: DNS"<<endl;
                                cout<<"     +Protocolo:  TCP/UDP"<<endl;
                                bandera_DNS = 1;

                            }
                            if(ayuda_int==67){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==68){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==69){
                                cout<<"     +Servicio: TFTP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_int==80){
                                cout<<"     +Servicio: HTTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==110){
                                cout<<"     +Servicio: POP3"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==143){
                                cout<<"     +Servicio: IMAP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==443){
                                cout<<"     +Servicio: HTTPS"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==993){
                                cout<<"     +Servicio: IMAP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_int==995){
                                cout<<"     +Servicio: POP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }

                        }
                        if(ayuda_int>=1024 and ayuda_int<= 49151){
                            cout<<"Puertos registrados"<<endl;
                        }
                        if(ayuda_int>=49152 and ayuda_int<=65535){
                            cout<<"Puertos dinamicos o privados"<<endl;
                        }
                        //puerto de destino
                        ayuda.clear();
                        for(i=0;i<=1;i++){
                            palabra = getc(archivo);
                            ayuda += binario_8bits(palabra);
                        }
                        unsigned int ayuda_double;
                        ayuda_double= bin_dec_double(ayuda);
                        cout<< "Puerto de destino: "<<ayuda_double<<endl;
                        cout<< "Puerto de destino catalogado: ";
                        if(ayuda_double>=0 and ayuda_double<= 1023){
                            cout<<"Puertos bien conocidos"<<endl;
                                if(ayuda_double==20){
                                cout<<"     +Servicio: FTP"<<endl;
                                cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_double==21){
                                cout<<"     +Servicio: FTP"<<endl;
                                cout<<"     +Protocolo:  FCP"<<endl;
                            }
                            if(ayuda_double==22){
                                cout<<"     +Servicio: SSH"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==23){
                                cout<<"     +Servicio: TELNET"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==25){
                                cout<<"     +Servicio: SMTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==53){
                                cout<<"     +Servicio: DNS"<<endl;
                                cout<<"     +Protocolo:  TCP/UDP"<<endl;
                                bandera_DNS = 1;
                            }
                            if(ayuda_double==67){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_double==68){
                                cout<<"     +Servicio: DHCP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_double==69){
                                cout<<"     +Servicio: TFTP"<<endl;
                                cout<<"     +Protocolo:  UDP"<<endl;
                            }
                            if(ayuda_double==80){
                                cout<<"     +Servicio: HTTP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==110){
                                cout<<"     +Servicio: POP3"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==143){
                                cout<<"     +Servicio: IMAP"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==443){
                                cout<<"     +Servicio: HTTPS"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==993){
                                cout<<"     +Servicio: IMAP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double==995){
                                cout<<"     +Servicio: POP SSL"<<endl;
                                cout<<"     +Protocolo:  TCP"<<endl;
                            }
                            if(ayuda_double == 53){
                                cout<<"Servicio: DNS"<< endl << "Protocolo: TCP/UDP";
                            }
                        }
                        if(ayuda_double>=1024 and ayuda_double<= 49151){
                            cout<<"Puertos registrados"<<endl;
                        }
                        if(ayuda_double>=49152 and ayuda_double<=65535){
                            cout<<"Puertos dinamicos o privados"<<endl;
                        }

                        //longitud total
                        cout<<endl;
                        cout<<"------Longitud total-------"<<endl;
                        ayuda.clear();
                        auxiliar.clear();
                        for(i=0;i<=1;i++){
                            palabra = getc(archivo);
                            ayuda += binario_8bits(palabra);
                        }
                        ayuda_int = bin_dec_double(ayuda);
                        cout<<"Longitud total es:"<< ayuda_int<<endl;

                        //suma de verificacion/ checksum

                        cout<<"--------Suma de verificacion--------"<<endl;
                        cout << "Suma de verificacion: ";
                        for(i=0;i<=1;i++){
                            palabra = getc(archivo);
                            printf("%x:",palabra);
                        }
                        cout<<endl;
                        //dns
                        if(bandera_DNS==1){
                            unsigned int QDcount, ANcount, NScount, ARcount;
                            cout<<"---------------DNS----------------"<<endl;
                            ayuda.clear();
                            auxiliar.clear();

                            bandera_DNS = 0;
                            cout<<"--------------ID-----------------"<<endl;
                            for(i=0;i<=1;i++){
                                palabra = fgetc(archivo);
                                printf("%x:",palabra);
                            }
                            cout<<endl;
                            for(i=0;i<=1;i++){
                                palabra = fgetc(archivo);
                                auxiliar += binario_8bits(palabra);
                            }
                            cout<<"-------------BANDERAS------------"<<endl;
                            cout<<"QR: ";
                            ayuda=auxiliar[0];
                            if(ayuda=="0"){
                                cout<<"Consulta"<<endl;
                            }
                            if(ayuda=="1"){
                                cout<<"Respuesta "<<endl;
                            }
                            ayuda.clear();
                            cout<<"-----------Op Code----------------"<<endl;
                            for(i=1;i<=4;i++){
                                ayuda += auxiliar[i];
                            }
                            if(bin_dec(ayuda)==0){
                                cout<<"0 Consulta estandar (QUERY)"<<endl;
                            }
                            if(bin_dec(ayuda)==1){
                                cout<<"1 Consulta inversa (QUERY)"<<endl;
                            }
                            if(bin_dec(ayuda)==2){
                                cout<<"2 Solicitud del estado del servidor(STATUS)"<<endl;
                            }
                            cout<<endl;
                            ayuda.clear();
                            cout<<"------------AA-----------"<<endl;
                            ayuda = auxiliar[5];
                            cout<<"AA: ";
                            if(ayuda=="0"){
                                cout<<"Desactivado"<<endl;
                            }
                            if(ayuda=="1"){
                                cout<<"activado"<<endl;
                            }
                            ayuda.clear();
                            ayuda = auxiliar[5];
                            cout<<"--------------TC-----------"<<endl;
                            cout<<"TC: ";
                            if(ayuda=="0"){
                                cout<<"Desactivado"<<endl;
                            }
                            if(ayuda=="1"){
                                cout<<"activado"<<endl;
                            }
                            ayuda.clear();
                            ayuda = auxiliar[6];
                            cout<<"--------------RD-----------"<<endl;
                            cout<<"RD: ";
                            if(ayuda=="0"){
                                cout<<"Desactivado"<<endl;
                            }
                            if(ayuda=="1"){
                                cout<<"activado"<<endl;
                            }
                            ayuda.clear();
                            ayuda = auxiliar[7];
                            cout<<"--------------RA-----------"<<endl;
                            cout<<"RA: ";
                            if(ayuda=="0"){
                                cout<<"Desactivado"<<endl;
                            }
                            if(ayuda=="1"){
                                cout<<"activado"<<endl;
                            }
                            ayuda.clear();
                            cout<<"----------Rcode----------"<<endl;

                            for(i=12;i<=15;i++){
                                ayuda += auxiliar[i];
                            }
                            if(bin_dec(ayuda)==0){
                                cout<<"0. Ningun error."<<endl;
                            }
                            if(bin_dec(ayuda)==1){
                                cout<<"1. Error de formato. El servidor fue incapaz de interpretar el mensaje."<<endl;
                            }
                            if(bin_dec(ayuda)==2){
                                cout<<"2. Fallo en el servidor. El mensaje no fue procesado debido a un problema con el servidor."<<endl;
                            }
                            if(bin_dec(ayuda)==3){
                                cout<<"3. Error en nombre. El nombre de dominio de la consulta no existe. Solo valido si el bit AA esta activo en la respuesta."<<endl;
                            }
                            if(bin_dec(ayuda)==4){
                                cout<<"4. No implementado. El tipo solicitado de consulta no esta implementado en el servidor de nombres."<<endl;
                            }
                            if(bin_dec(ayuda)==5){
                                cout<<"5. Rechazado. El servidor rechaza responder por razones politicas. Los demas valores se reservan para su usuario en el futuro."<<endl;
                            }
                            ayuda.clear();
                            auxiliar.clear();
                            cout<<"------CONTADORES------"<<endl;
                            cout<<"------QDcount------"<<endl;
                            for(i=0;i<=1;i++){
                                palabra = getc(archivo);
                                ayuda += binario_8bits(palabra);
                            }
                            QDcount = bin_dec(ayuda);
                            cout<<"QDcount: "<<QDcount<<endl;
                            ayuda.clear();

                            cout<<"------ANcount------"<<endl;
                            for(i=0;i<=1;i++){
                                palabra = getc(archivo);
                                ayuda += binario_8bits(palabra);
                            }
                            ANcount = bin_dec(ayuda);
                            cout<<"ANcount: "<<ANcount<<endl;
                            ayuda.clear();

                            cout<<"------NScount------"<<endl;
                            for(i=0;i<=1;i++){
                                palabra = getc(archivo);
                                ayuda += binario_8bits(palabra);
                            }
                            NScount = bin_dec(ayuda);
                            cout<<"NScount: "<<NScount<<endl;
                            ayuda.clear();

                            cout<<"------ARcount------"<<endl;
                            for(i=0;i<=1;i++){
                                palabra = getc(archivo);
                                ayuda += binario_8bits(palabra);
                            }
                            ARcount = bin_dec(ayuda);
                            cout<<"ARcount: "<<ARcount<<endl;
                            ayuda.clear();

                            cout<<"-------nombre del dominio------"<<endl;
                            auxiliar.clear();
                            do{
                                palabra = fgetc(archivo);


                                if(palabra == 0){
                                    break;
                                }
                                auxiliar += palabra;
                                if(!isalpha(palabra))
                                {
                                    cout<<".";
                                    auxiliar += ".";
                                }
                                printf("%c",palabra);

                            }while(true);

                            cout<<endl;
                            string link;
                            link = auxiliar;

                            cout<<"------tipo-----"<<endl;
                            auxiliar.clear();
                            ayuda.clear();
                            for(i=0;i<=1;i++){
                                palabra = fgetc(archivo);
                                ayuda += binario_8bits(palabra);
                            }
                            cout<<"tipo: ";
                            ayuda_int = bin_dec(ayuda);
                            if(ayuda_int==1){
                                cout<<"A"<<endl;
                            }
                            if(ayuda_int==5){
                                cout<<"CNAME"<<endl;
                            }
                            if(ayuda_int==13){
                                cout<<"HINFO"<<endl;
                            }
                            if(ayuda_int==15){
                                cout<<"MX"<<endl;
                            }
                            if(ayuda_int==22 or ayuda_int==23){
                                cout<<"NS"<<endl;
                            }

                            cout<<"------clase-----"<<endl;
                            auxiliar.clear();
                            ayuda.clear();
                            for(i=0;i<=1;i++){
                                palabra = fgetc(archivo);
                                ayuda += binario_8bits(palabra);
                            }
                            cout<<"clase: ";
                            ayuda_int= bin_dec(ayuda);
                            if(ayuda_int==1){
                                cout<<"IN"<<endl;
                            }
                            if(ayuda_int==3){
                                cout<<"CH"<<endl;
                            }

                            if(ANcount!=0)
                            {
                                cout<<"-----Respuesta-----"<<endl;
                                //agregamos el fseek
                                //fseek(archivo,)
                                //continuacion de respuesta
                                cout<<"-------------nombre del dominio--------------"<<endl;
                                cout<<link<<endl;
                                cout<<"------tipo-----"<<endl;
                                auxiliar.clear();
                                ayuda.clear();
                                for(i=0;i<=1;i++)
                                {
                                    palabra = fgetc(archivo);
                                    ayuda += binario_8bits(palabra);
                                }
                                int tipo_respuesta;

                                cout<<"tipo: ";
                                ayuda_int = bin_dec(ayuda);
                                tipo_respuesta = ayuda_int;
                                if(ayuda_int==1){
                                    cout<<"A"<<endl;
                                }
                                if(ayuda_int==5){
                                    cout<<"CNAME"<<endl;
                                }
                                if(ayuda_int==13){
                                    cout<<"HINFO"<<endl;
                                }
                                if(ayuda_int==15){
                                    cout<<"MX"<<endl;
                                }
                                if(ayuda_int==22 or ayuda_int==23){
                                    cout<<"NS"<<endl;
                                }

                                cout<<"------clase-----"<<endl;
                                auxiliar.clear();
                                ayuda.clear();
                                for(i=0;i<=1;i++){
                                    palabra = fgetc(archivo);
                                    ayuda += binario_8bits(palabra);
                                }
                                cout<<"clase: ";
                                ayuda_int= bin_dec(ayuda);
                                if(ayuda_int==1){
                                    cout<<"IN"<<endl;
                                }
                                if(ayuda_int==3){
                                    cout<<"CH"<<endl;
                                }

                                cout<<"----------CLASE------------"<<endl;
                                auxiliar.clear();
                                ayuda.clear();
                                for(i=0;i<=1;i++){
                                    palabra = getc(archivo);
                                    ayuda += binario_8bits(palabra);
                                }
                                if(bin_dec(ayuda)==1){
                                    cout<<"IN: relacionada a protocolos de Internet, y por lo tanto, es el sistema que utilizaremos en nuestro caso"<<endl;
                                }
                                if(bin_dec(ayuda)==3){
                                    cout<<"CH: para el sistema ca??tico"<<endl;
                                }
                                ayuda.clear();
                                cout<<"----------TTL--------------"<<endl;

                                for(i=0;i<=3;i++){
                                    palabra = getc(archivo);
                                    ayuda += binario_8bits(palabra);
                                }
                                cout<<"TTL: "<< bin_dec(ayuda);
                                ayuda.clear();
                                cout<<"--------Longitud de datos------"<<endl;
                                for(i=0;i<=1;i++){
                                    palabra = getc(archivo);
                                    ayuda += binario_8bits(palabra);
                                }
                                cout<<"Longitud de datos: "<<bin_dec(ayuda)<<endl;
                                ayuda.clear();
                                cout<<"-----------RDATA---------"<<endl;
                                if(tipo_respuesta==1){
                                    cout<<"A: ";
                                    ayuda.clear();
                                    for(i=0;i<=3;i++){
                                        palabra = fgetc(archivo);
                                        cout<< bin_dec(binario_8bits(palabra))<<".";
                                    }
                                    cout<<endl;
                                }
                                if(tipo_respuesta==5){
                                    cout<<"CNAME: "<<endl;
                                }
                                if(tipo_respuesta==13){
                                    cout<<"HINFO:"<<endl;
                                }
                                if(tipo_respuesta==15){
                                    cout<<"MX: "<<endl;
                                }
                                if(tipo_respuesta==22 or tipo_respuesta==23){
                                    cout<<"NS: "<<endl;
                                }
                            }
                        }
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
                        cout<<"Procesando llamada cr???tica y de emergencia."<<endl;
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

                    //tama??o de datos
                    ayuda.clear();
                    auxiliar.clear();
                    auxiliar_apoyo.clear();

                    for(i=0;i<=1;i++){
                        palabra = fgetc(archivo);
                        auxiliar += binario_8bits(palabra);
                    }
                    cout<<"Tama??o de datos: "<<bin_dec(auxiliar)<<endl;

                    //encabezado siguiente
                    ayuda.clear();
                    auxiliar.clear();
                    auxiliar_apoyo.clear();
                    int encabezado_sig;

                    palabra = getc(archivo);
                    auxiliar = binario_8bits(palabra);
                    ayuda_int = bin_dec(auxiliar);
                    cout<<"Encabezado siguiente: ";
                    encabezado_sig = bin_dec(auxiliar);
                    if(encabezado_sig == 58){
                        cout<< "58. ICMPv6"<<endl;
                    }
                    if(encabezado_sig== 1){
                        cout<< "ICMP v4 no es posible"<<endl;
                    }
                    if(encabezado_sig == 6){
                        cout<< "TCP no es posible"<<endl;
                    }
                    if(encabezado_sig== 17){
                        cout<< "UDP no es posible"<<endl;
                    }
                    if(encabezado_sig == 118){
                        cout<< "STP no es posible"<<endl;
                    }
                    if(encabezado_sig == 121){
                        cout<< "SMP no es posible"<<endl;
                    }

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


                    //ICMPv6

                    if(encabezado_sig == 58){
                        int tipo_icmp = 0;
                        cout<<endl<< "----------ICMPv6-------"<<endl;
                        //tipo
                        palabra = getc(archivo);
                        ayuda.clear();
                        auxiliar.clear();
                        ayuda = binario_8bits(palabra);
                        tipo_icmp = bin_dec(ayuda);
                        cout<< "Tipo: " << tipo_icmp <<endl;
                        ayuda.clear();

                        //codigo
                        int codigo_icmp;
                        palabra = getc(archivo);
                        ayuda = binario_8bits(palabra);
                        codigo_icmp = bin_dec(ayuda);
                        cout<< "El codigo es: "<< codigo_icmp<< endl;
                        ayuda.clear();

                        //ifs anidados para tablas
                        if(tipo_icmp == 1){
                            cout<<"Mensaje de destino inalcanzable"<<endl;
                            if(codigo_icmp==0){
                                cout<<"No existe ruta de destino"<<endl;
                            }
                            if(codigo_icmp==1){
                                cout<<"Comunicacion con el destino administrativamente prohibida"<<endl;
                            }
                            if(codigo_icmp==2){
                                cout<<"No asignado"<<endl;
                            }
                            if(codigo_icmp==3){
                                cout<<"Direccion inalcanzable"<<endl;
                            }
                        }
                        if(tipo_icmp == 2){
                            cout<<"Mensaje de paquetes demasiado grande"<<endl;

                        }
                        if(tipo_icmp == 3){
                            cout<< "time exeded Message"<<endl;
                            if(codigo_icmp==0){
                                cout<<"El limite del salto excedido"<<endl;
                            }
                            if(codigo_icmp==1){
                                cout<<"Tiempo de Reensamble de fragmento excedido"<<endl;
                            }
                        }
                        if(tipo_icmp == 4){
                            cout<<"Mensaje de problema de parametro"<<endl;
                            if(codigo_icmp==0){
                                cout<<"el campo de encabezado erroneo encontro"<<endl;
                            }
                            if(codigo_icmp==1){
                                cout<<"El tipo siguiente desconocido de la encabezado encontro"<<endl;
                            }
                            if(codigo_icmp==2){
                                cout<<"opcion desconocida del IPv6 encontrada"<<endl;
                            }
                        }
                        if(tipo_icmp==128){
                            cout<<"Mensaje del pedido de eco"<<endl;
                        }
                        if(tipo_icmp==129){
                            cout<< "Mensaje de respuesta de eco"<<endl;
                        }
                        if(tipo_icmp==133){
                            cout<<"Mensaje de solicitud del router"<<endl;
                        }
                        if(tipo_icmp==134){
                            cout<<"Mensaje de anuncio del router"<<endl;
                        }
                        if(tipo_icmp==135){
                            cout<<"Mensaje de solicitud vecino"<<endl;
                        }
                        if(tipo_icmp==136){
                            cout<<"Mensaje de anuncio de vecino"<<endl;
                        }
                        if(tipo_icmp==137){
                            cout<<"Reoriente el mensaje"<<endl;
                        }




                        //checksum
                        cout<<endl;
                        cout<<"--------Checksum--------"<<endl;
                        for(i=0;i<=1;i++){
                            palabra = getc(archivo);
                            printf("%x:",palabra);

                        }
                        cout<<endl;

                    }
                    if(encabezado_sig== 1){
                        cout<< "ICMP v4 no es posible"<<endl;
                    }
                    if(encabezado_sig == 6){
                        cout<< "TCP no es posible"<<endl;
                    }
                    if(encabezado_sig== 17){
                        cout<< "UDP no es posible"<<endl;
                    }
                    if(encabezado_sig == 118){
                        cout<< "STP no es posible"<<endl;
                    }
                    if(encabezado_sig == 121){
                        cout<< "SMP no es posible"<<endl;
                    }
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
        remove("datos.bin");

}
return (0);}
