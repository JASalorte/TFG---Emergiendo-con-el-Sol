/* 
 * File:   sshFunctions.h
 * Author: Linkku
 *
 * Created on 6 de mayo de 2015, 19:04
 */

#ifndef SSHFUNCTIONS_H
#define	SSHFUNCTIONS_H

using namespace std;

int verify_knownhost(ssh_session session) {
    int state, hlen;
    unsigned char *hash = NULL;
    char *hexa;
    char buf[10];
    state = ssh_is_server_known(session);
    hlen = ssh_get_pubkey_hash(session, &hash);
    if (hlen < 0)
        return -1;
    switch (state) {
        case SSH_SERVER_KNOWN_OK:
            break; /* ok */
        case SSH_SERVER_KNOWN_CHANGED:
            fprintf(stderr, "Host key for server changed: it is now:\n");
            ssh_print_hexa("Public key hash", hash, hlen);
            fprintf(stderr, "For security reasons, connection will be stopped\n");
            free(hash);
            return -1;
        case SSH_SERVER_FOUND_OTHER:
            fprintf(stderr, "The host key for this server was not found but an other"
                    "type of key exists.\n");
            fprintf(stderr, "An attacker might change the default server key to"
                    "confuse your client into thinking the key does not exist\n");
            free(hash);
            return -1;
        case SSH_SERVER_FILE_NOT_FOUND:
            fprintf(stderr, "Could not find known host file.\n");
            fprintf(stderr, "If you accept the host key here, the file will be"
                    "automatically created.\n");
            /* fallback to SSH_SERVER_NOT_KNOWN behavior */
        case SSH_SERVER_NOT_KNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            fprintf(stderr, "The server is unknown. Do you trust the host key?\n");
            fprintf(stderr, "Public key hash: %s\n", hexa);
            fprintf(stderr, "(yes/no): ");
            free(hexa);
            if (fgets(buf, sizeof (buf), stdin) == NULL) {
                free(hash);
                return -1;
            }
            if (strncasecmp(buf, "yes", 3) != 0 || strncasecmp(buf, "Yes", 3) != 0) {
                free(hash);
                return -1;
            }
            if (ssh_write_knownhost(session) < 0) {
                fprintf(stderr, "Error %s\n", strerror(errno));
                free(hash);
                return -1;
            }
            break;
        case SSH_SERVER_ERROR:
            fprintf(stderr, "Error %s", ssh_get_error(session));
            free(hash);
            return -1;
    }
    free(hash);
    return 0;
}

int authenticate_pubkey(ssh_session session) {
    int rc;
    rc = ssh_userauth_publickey_auto(session, NULL, "Pablito");
    if (rc == SSH_AUTH_ERROR) {
        fprintf(stderr, "Authentication failed: %s\n", ssh_get_error(session));
        return SSH_AUTH_ERROR;
    }
    return rc;
}

int shell_session(ssh_session session) {

    ssh_channel channel;
    int rc;
    channel = ssh_channel_new(session);

    if (channel == NULL)
        return SSH_ERROR;
    rc = ssh_channel_open_session(channel);

    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        return rc;
    }

    rc = ssh_channel_request_exec(channel, "ls -l");
    if (rc != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return rc;
    }

    char buffer[256];
    unsigned int nbytes;
    nbytes = ssh_channel_read(channel, buffer, sizeof (buffer), 0);

    while (nbytes > 0) {
        if (fwrite(buffer, 1, nbytes, stdout) != nbytes) {
            ssh_channel_close(channel);
            ssh_channel_free(channel);
            return SSH_ERROR;
        }
        nbytes = ssh_channel_read(channel, buffer, sizeof (buffer), 0);
    }

    if (nbytes < 0) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return SSH_ERROR;
    }


    ssh_channel_close(channel);
    ssh_channel_send_eof(channel);
    ssh_channel_free(channel);
    return SSH_OK;

}

//int scp_helloworld(ssh_session session, ssh_scp scp) {
//    int rc;
//    const char *helloworld = "Hello, world!\n";
//    int length = strlen(helloworld);
//    rc = ssh_scp_push_directory(scp, "helloworld", S_IRWXU);
//    if (rc != SSH_OK) {
//        fprintf(stderr, "Can't create remote directory: %s\n",
//                ssh_get_error(session));
//        return rc;
//    }
//    rc = ssh_scp_push_file
//            (scp, "helloworld.txt", length, S_IRUSR | S_IWUSR);
//    if (rc != SSH_OK) {
//        fprintf(stderr, "Can't open remote file: %s\n",
//                ssh_get_error(session));
//        return rc;
//    }
//    rc = ssh_scp_write(scp, helloworld, length);
//    if (rc != SSH_OK) {
//        fprintf(stderr, "Can't write to remote file: %s\n",
//                ssh_get_error(session));
//        return rc;
//    }
//    return SSH_OK;
//}

int scp_write(ssh_session session) {
    ssh_scp scp;
    int rc;
    scp = ssh_scp_new
            (session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
    if (scp == NULL) {
        fprintf(stderr, "Error allocating scp session: %s\n", ssh_get_error(session));
        return SSH_ERROR;
    }
    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error initializing scp session: %s\n",
                ssh_get_error(session));
        ssh_scp_free(scp);
        return rc;
    }

    //scp_helloworld(session, scp);

    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return SSH_OK;
}

void error(const char *s) {
    /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
    perror(s);
    exit(EXIT_FAILURE);
}

//void procesoArchivo(char *archivo)
//{
//  /* Para "procesar", o al menos, hacer algo con el archivo, vamos a decir su tama�o en bytes */
//  /* para ello haremos lo que vemos aqu�: http://totaki.com/poesiabinaria/2010/04/tamano-de-un-fichero-en-c/ */
//  FILE *fich;
//  long ftam;
//
//  fich=fopen(archivo, "r");
//  if (fich)
//    {
//      fseek(fich, 0L, SEEK_END);
//      ftam=ftell(fich);
//      fclose(fich);
//      /* Si todo va bien, decimos el tama�o */
//      printf ("%30s (%ld bytes)\n", archivo, ftam);
//    }
//  else
//    /* Si ha pasado algo, s�lo decimos el nombre */
//    printf ("%30s (No info.)\n", archivo);
//}

int send_file(const char* dir, const char* filenameOrig, const char* folderDest, ssh_session session) {

    streampos size;
    char * memblock;

    string fulldir = "";
    fulldir.append(dir);
    fulldir.append("\\");
    fulldir.append(filenameOrig);


    ifstream myfile(fulldir.c_str(), ios::in | ios::binary | ios::ate);
    if (myfile.is_open()) {
        size = myfile.tellg();
        memblock = new char [size];
        myfile.seekg(0, ios::beg);
        myfile.read(memblock, size);
        myfile.close();

        //cout << "the entire file content is in memory";



        ssh_scp scp;
        int rc;
        scp = ssh_scp_new
                (session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
        if (scp == NULL) {
            qDebug("1");
            fprintf(stderr, "Error allocating scp session: %s\n", ssh_get_error(session));
            return SSH_ERROR;
        }
        rc = ssh_scp_init(scp);
        if (rc != SSH_OK) {
            qDebug("2");
            fprintf(stderr, "Error initializing scp session: %s\n", ssh_get_error(session));
            ssh_scp_free(scp);
            return rc;
        }

        //int rc;
        //const char *helloworld = "Hello, world!\n";
        //int length = strlen(helloworld);

        rc = ssh_scp_push_directory(scp, folderDest, S_IRWXU);
        if (rc != SSH_OK) {
            qDebug(ssh_get_error(session));
            //fprintf(stderr, "Can't create remote directory: %s\n",  ssh_get_error(session));
            return rc;
        }

        rc = ssh_scp_push_file(scp, filenameOrig, size, S_IRUSR | S_IWUSR);
        if (rc != SSH_OK) {
            qDebug("4");
            //fprintf(stderr, "Can't open remote file: %s\n",  ssh_get_error(session));
            return rc;
        }
        rc = ssh_scp_write(scp, memblock, size);
        if (rc != SSH_OK) {
            qDebug("5");
            //fprintf(stderr, "Can't write to remote file: %s\n",    ssh_get_error(session));
            return rc;
        }

        ssh_scp_close(scp);
        ssh_scp_free(scp);


        delete[] memblock;
    }


    return SSH_OK;
}

int send_dir_allfiles(const char* directory, const char* folderDest, ssh_session session) {

    /* Con un puntero a DIR abriremos el directorio */
    DIR *dir;
    /* en *ent habr� informaci�n sobre el archivo que se est� "sacando" a cada momento */
    struct dirent *ent;

    /* Empezaremos a leer en el directorio actual */
    dir = opendir(directory);

    /* Miramos que no haya error */
    if (dir == NULL)
        error("No puedo abrir el directorio");

    /* Una vez nos aseguramos de que no hay error, �vamos a jugar! */
    /* Leyendo uno a uno todos los archivos que hay */
    while ((ent = readdir(dir)) != NULL) {
        /* Nos devolver� el directorio actual (.) y el anterior (..), como hace ls */
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) {
            /* Una vez tenemos el archivo, lo pasamos a una funci�n para procesarlo. */
            //procesoArchivo(ent->d_name);
            send_file(directory, ent->d_name, folderDest, session);
        }
    }
    closedir(dir);

    return SSH_OK;


    //    ssh_scp scp;
    //    int rc;
    //    scp = ssh_scp_new(session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
    //    if (scp == NULL) {
    //        fprintf(stderr, "Error allocating scp session: %s\n", ssh_get_error(session));
    //        return SSH_ERROR;
    //    }
    //    rc = ssh_scp_init(scp);
    //    if (rc != SSH_OK) {
    //        fprintf(stderr, "Error initializing scp session: %s\n",
    //                ssh_get_error(session));
    //        ssh_scp_free(scp);
    //        return rc;
    //    }
    //
    //    //scp_helloworld(session, scp);
    //
    //    ssh_scp_close(scp);
    //    ssh_scp_free(scp);
    //    return SSH_OK;
}

#endif	/* SSHFUNCTIONS_H */

