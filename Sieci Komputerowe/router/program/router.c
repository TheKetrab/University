// Bartlomiej Grochowski 300 951

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "router.h"
#include "entry.h"
#include "util.h"

void insert_entry(struct router* r, struct entry* e) {

    // pusta lista
    if (r->last_entry == NULL) {
        r->last_entry = e;
        r->first_entry = e;
    }

    // 1 lub wiele elementow
    else {
        r->last_entry->next = e;
        r->last_entry = e;
    }

    r->size++;
}


void delete_entry(struct router* r, struct entry* e) {
    if (delete_entry_check(r,e)) r->size--;
}

// zwraca 1, jesli usunieto, 0 jesli nie usunieto
int delete_entry_check(struct router* r, struct entry* e) {

    // pusta lista
    if (r->last_entry == NULL) return 0;

    // jesli trzeba usunac pierwszy element
    if (cmp_entries(r->first_entry,e)) {

        // jesli to lista jednoelementowa
        if (r->first_entry == r->last_entry) {
            r->last_entry = NULL;
            free(r->first_entry);
            r->first_entry = NULL;
            return 1;
        }

        struct entry* to_remove = r->first_entry;
        r->first_entry = r->first_entry->next;
        free(to_remove);
        return 1;
    }

    // else zacznij od drugiego i szukaj
    struct entry* temp = r->first_entry->next;
    struct entry* prev = r->first_entry;
    while ( !cmp_entries(temp,e) ) {
        prev = temp;
        temp = temp->next;
        if (temp == NULL) return 0;
    }

    // doszedles do szukanego do usuniecia

    // jesli to ostatni element
    if (temp == r->last_entry) {
        r->last_entry = prev;
        prev->next = NULL;
        free(temp);
        return 1;
    }

    // usuwany element to jakis srodkowy
    prev->next = temp->next;
    free(temp);
    return 1;

}

void print_entries(struct router* r, _Bool debug) {

    struct entry* temp = r->first_entry;
    while (temp != NULL) {
        debug ? print_entry_debug(temp) : print_entry(temp);
        temp = temp->next;
    }

}

// funkcja wysyla wszystkim swoim sasiadom swoja tablice routingu
// a dokladniej n pakietow UDP, gdzie n to liczba wpisow w tablicy routingu, ktore sa direct
// all = true -> wyslij wszystkie wpisy
// all = false -> tylko jeden wpis (e)
void share_route_table(struct router* r, _Bool all, struct entry* e) {

    if (r->first_entry == NULL) return;
    for (struct entry* temp = r->first_entry; temp!=NULL; temp=temp->next) {

        // wysylaj tylko do bezposrednich
        if (temp->direct) {

            char* broadcast_address = ip_to_string(ip_to_broadcast(temp->web_ip,temp->mask));
            struct sockaddr_in router_address;
            bzero (&router_address, sizeof(router_address));
            router_address.sin_family      = AF_INET;
            router_address.sin_port        = htons(PORT);
            inet_pton(AF_INET, broadcast_address, &router_address.sin_addr);

            struct entry* START = all ? r->first_entry : e;
            struct entry* FINISH = all ? NULL : e->next;
            
            // wyslij wszystkie swoje wpisy w osobnych pakietach
            for (struct entry* temp2 = START; temp2!=FINISH; temp2=temp2->next) {

                char* message = create_udp_packet(temp2);
                ssize_t message_len = 9;
                int bytes_sent = sendto(
                    r->sockfd,
                    message,
                    message_len,
                    0,
                    (struct sockaddr*) &router_address,
                    sizeof(router_address)
                );

                //printf("send to %s: (%s , %u)\n",ip_to_string(temp->web_ip),ip_to_string(temp2->web_ip),temp2->dist);

                if (bytes_sent != message_len) {
                    set_unreachable(temp2);
                }
            } // inner for
        } // end direct
    } // outer for

}

// zwraca 1 jesli sukces
// 0 jesli przyszlo info od nieosiagalnego kogosc albo od siebie samego
// -1 jesli jakis blad z gniazda
int get_and_update(struct router* r, int round) {
    
    struct sockaddr_in  sender;	
    socklen_t           sender_len = sizeof(sender);
    u_int8_t            buffer[IP_MAXPACKET+1];

    ssize_t datagram_len = recvfrom(
        r->sockfd,
        buffer,
        IP_MAXPACKET,
        MSG_DONTWAIT,
        (struct sockaddr*)&sender,
        &sender_len
    );


    if (datagram_len < 0) {
        fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
        return -1;
    }

    if (datagram_len == 0) {
        return 0;
    }

    // odebrano cos
    char sender_ip_str[20];
    inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
    uint32_t udp_ip; uint8_t udp_mask; uint32_t udp_dist;
    spread_udp_packet((char*)buffer,&udp_ip,&udp_mask,&udp_dist);

    struct entry* received_entry = create_entry(udp_ip,udp_mask,udp_dist);
    uint32_t sender_ip = ntohl((uint32_t)sender.sin_addr.s_addr);
    
    //printf("receive from %s: (%s , %u)\n",ip_to_string(sender_ip),ip_to_string(received_entry->web_ip),received_entry->dist);

    // czy to info jest o routerze polaczonym bezposrednio?
    if (is_ip_in_direct_web(r,sender_ip)) received_entry->direct = 1;
    else                                  received_entry->direct = 0;

    uint32_t dist_to_sender = get_dist_to_ip(r,sender_ip);

    _Bool found = 0;
    for (struct entry* temp=r->first_entry; temp!=NULL; temp=temp->next) {
        if (cmp_entries(temp,received_entry)) {
            found = 1; // przyszlo info o wpisie, ktory mamy
            actualize(temp,received_entry,sender_ip,dist_to_sender,round);
            free(received_entry); // zwolnij pamiec
        }
    }

    // jesli przyszlo jakies info, ktorego nie mamy
    // o routerze nieosiagalnym, to go nie chcemy dodawac
    if (!get_reachable(received_entry)) {
        found = 1;
        free(received_entry);
    }

    // dodajemy nowy wpis do tablicy jesli nie znaleziono go we wpisie
    if (!found) {
        received_entry->dist += dist_to_sender;
        insert_entry(r,received_entry);
        received_entry->round = round;
        received_entry->via_ip = sender_ip;
        received_entry->direct = 0; // gdyby byl direct, to bysmy go dodali na wejsciu z pliku
    }

    return 1;
}

// temp - wpis routera o tej sieci
// received_entry - przyslany przez jakis router wpis o tej sieci
void actualize(struct entry* temp, struct entry* received_entry, uint32_t sender_ip, uint32_t dist_to_sender, int round) {

    // jesli przyszlo info od sendera o routerze osiagalnym
    if (get_reachable(received_entry)) {

        if (temp->direct) {

            // nadeslal ktos z tej sieci bezposredniej (mozliwe ze sam sobie)
            // czyli siec jest osiagalna -> zaktualizuj
            // czyli ten przypadek oznacza rowniez odzyskanie polaczenia bezposredniego
            if (is_ip_in_web(temp,sender_ip)) {
                temp->round = round;
                temp->dist = temp->basic_dist;
                temp->via_ip = 0;
                return;
            }

            uint32_t new_dist = received_entry->dist + dist_to_sender;

            // jesli wpis byl direct i nieosiagalny to idz przez tego kto ci wyslal
            if (!get_reachable(temp)) {
                temp->via_ip = sender_ip;
                temp->round = round;
                temp->dist = new_dist;
                return;
            }

            // jesli byl direct i osiagalny to relaksacja
            if (get_reachable(temp)) {
                typical_check(temp,sender_ip,new_dist,round);
                return;
            }
        }

        else { // !temp->direct
            uint32_t new_dist = received_entry->dist + dist_to_sender;
            typical_check(temp,sender_ip,new_dist,round);
            return;
        }
    }

    // przyszlo info od sendera o routerze nieosiagalnym
    else {

        if (temp->direct) {

            // jesli przyslano wpis o direct to byc moze nastapilo odzyskanie polaczenia bezposredniego
            if (is_ip_in_web(temp,sender_ip)) {
                temp->round = round;
                temp->dist = temp->basic_dist;
                temp->via_ip = 0;
                return;
            }

        }

        // jesli ten przez kogo ja ide stracil osiagalnosc tej sieci, to ja tez trace
        if (temp->via_ip == sender_ip) {
            set_unreachable(temp);
            return;
        }
    }

}



void typical_check(struct entry* temp, uint32_t sender_ip, uint32_t new_dist, int round) {

    if (temp->via_ip == sender_ip) {

        // zliczanie do nieskonczonosci
        if (new_dist > MAX) {
            set_unreachable(temp);
        } else {
            temp->dist = new_dist;
            temp->round = round;
        }
    }

    else { // temp->via_ip != sender_ip
        
        // sprobuj relaksacji
        if (new_dist < temp->dist) { // relaksacja
            temp->dist = new_dist;
            temp->via_ip = sender_ip;
            temp->round = round;
        }
    }
}




int read_web_from_file(struct router* r, char* filename) {

    int result = 0;

    FILE *f  = fopen(filename, "r"); // read only 
    if (f == NULL) {
    	fprintf(stderr,"failed to open file '%s'\n",filename); 
        return -1;
    }

    int n=0;
    char c=0;

    // READ N
    while ((c = fgetc(f)) != '\n') {
        n = n*10 + (c-'0');
    }

    // READ WEB INFO
    int read = 0;
    while (1) {

        uint8_t ip[5] = {0,0,0,0,0}; // ip i maska
        uint32_t dist = 0;
        int spaces = 0;
        int part = 0;

        // READ SINGLE WEB INFO
        while (1) {
            c = fgetc(f);

            if (c == EOF) {
                if (read >= n) { result = 1; goto after_reading; }
                if (read == n-1 && spaces == 2) { result=1; read++; break; }
             	fprintf(stderr,"unexpected end of file 'input.txt'\n"); 
                result = -1; goto after_reading;
            }

            if (c == '\n') { read++; break; }
            if (c == ' ') { spaces++; continue; }
            if (c == '.' || c == '/') { part++; continue; }
            if (spaces == 0) ip[part] = ip[part]*10 + (c-'0');
            if (spaces == 2) dist = dist*10 + (c-'0');
        }

        struct entry* e = create_entry(make_ip(ip[0],ip[1],ip[2],ip[3]),ip[4],dist);
        e->direct = 1;
        e->basic_dist = e->dist;
        e->round = 0;
        insert_entry(r,e);

        if (read >= n) break;
    }

after_reading:
            
    fclose(f);
    return result;
}

// iteruje sie po calej tablicy, jesli dlugo nie bylo
// aktualizacji to robi UNREACHABLE lub usuwa wpis 
void check_table(struct router* r, int round) {

    struct entry* to_remove[r->size];
    int n = 0; // ile usunac

    // aktualizowanie na unreachable i wychwytywanie wpisow do usuniecia (niepolaczonych bezposrednio)
    for (struct entry* temp = r->first_entry; temp!=NULL; temp=temp->next) {
        if (temp->round < round-6 && temp->direct==0) { to_remove[n]=temp; n++; }
        else if (temp->round < round-3) set_unreachable(temp);
    }

    // usuwanie
    for (int i=0; i<n; i++)
        delete_entry(r,to_remove[i]);
    
}

uint32_t get_dist_to_entry(struct router* r, struct entry* e) {

    for (struct entry* temp=r->first_entry; temp!=NULL; temp=temp->next)
        if (cmp_entries(temp,e))
            return temp->dist;

    // nie znaleziono -> unreachable
    return INF;
}

// zwraca DIST router r1 - router r2, gdzie r1 to r, a r2 ma ip ip 
// i oczywiscie porownuje r1 i r2 na dlugosci maski
uint32_t get_dist_to_ip(struct router* r, uint32_t ip) {

    if (is_self_ip(r,ip)) return 0;

    for (struct entry* temp=r->first_entry; temp!=NULL; temp=temp->next)
        if (is_ip_in_web(temp,ip))
            return temp->dist;

    // nie znaleziono -> unreachable
    return INF;
}

// czy to jest IP ktorejs z kart routera?
// takie ip jest tylko w direct
_Bool is_self_ip(struct router* r, uint32_t ip) {

    for (struct entry* temp=r->first_entry; temp!=NULL; temp=temp->next) {
        if (temp->direct && temp->web_ip == ip)
            return 1;
    }

    return 0;
}

// czy to IP nalezy do ktorejs sieci, do ktorej router jest bezposrednio polaczony?
_Bool is_ip_in_direct_web(struct router* r, uint32_t ip) {
    for (struct entry* temp = r->first_entry; temp!=NULL; temp=temp->next) {
        if (is_ip_in_web(temp,ip)) return 1;
    } return 0;
}
