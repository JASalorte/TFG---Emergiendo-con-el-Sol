/* 
 * File:   LinuxCalls.h
 * Author: Linkku
 *
 * Created on 21 de abril de 2015, 13:42
 */

#ifndef LINUXCALLS_H
#define	LINUXCALLS_H

std::vector<std::string> linux_return_function(const char* command) {
    FILE *fp = popen(command, "r");
    char buf[1024];

    std::vector<std::string> buffer;

    while (fgets(buf, 1024, fp)) {
        std::string a = buf;
        buffer.push_back(a);
    }

    fclose(fp);
    return buffer;
}

int createLinuxUser(const char* username){
    
    return 0;
}

#endif	/* LINUXCALLS_H */

