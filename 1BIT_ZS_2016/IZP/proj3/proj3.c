/**
*Autor: Alena Tesarova (xtesar36)
*Skupina: BIB (39)
*Datum: 4.12.2016
*
*Nazev: proj3.c
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t
{
    int id;
    float x;
    float y;
};

struct cluster_t
{
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL); //pokud c == NULL, program vypise chybovou hlasku
    assert(cap >= 0); //pokracuju, kdyz cap >=0
    c->capacity = cap;
    c->size = 0;
    if (cap == 0)
    {
        c->obj = NULL;
    }
    else
    {
        c->obj = malloc(sizeof(struct cluster_t)*cap);
    }

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    if (c->obj != NULL)
    {
        free(c->obj);
    }
    init_cluster(c,0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c->capacity==c->size) //pokud  jsme uz naplnili objekty na kapacitu, musime alokovat dalsi misto pro objekty
    {

        //realloc(c->obj,sizeof(struct obj_t)*c->capacity );
        c = resize_cluster(c,c->capacity+CLUSTER_CHUNK); //zvetsime cluster
        if (c == NULL)
        {
            fprintf(stderr,"we dont have enough space for an upgrade array of new capacity\n");
            return;
        }
    }
    if (c->obj != NULL)
    {
        c->obj[c->size] = obj;
        c->size++;
    }

}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
    for (int i = 0; i<c2->size; i++)
    {
        append_cluster(c1,c2->obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
    if (&carr[idx]!= NULL) //chci vedet co na nem je, primo data shluku - reference
    {
        clear_cluster(&carr[idx]);
        //potrebuju posunout vsechny objekty
        for (int i = idx + 1; i<narr; i++)
        {
            //zkopirovat na i1-1 vse co je na i
            carr[i-1]=carr[i];
        }
        return narr-1;

    }
    return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
    float a = abs(o1->x - o2->x);
    float b = abs(o1->y - o2->y);
    return sqrt(a*a+b*b);
}

/*
 Pocita vzdalenost dvou shluku.
 Na zaklade objektu, ktere jsou k sobe nejblize.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
    float max_vzdalenost = -1; //nikdy nemuze nastat / hledame nejvzdalenejsiho souseda
    float akt_vzdalenost = 0;
    for (int i = 0; i<(c1->size); i++)
    {
        for(int j = i; j<(c2->size); j++)
        {
            akt_vzdalenost = obj_distance(&c1->obj[i],&c2->obj[j]);
            if (akt_vzdalenost>max_vzdalenost)
            {
                max_vzdalenost = akt_vzdalenost;
            }
        }
    }
    return max_vzdalenost;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    float akt_vzdalenost = 0;
    float min_vzdalenost = 2000;
    for (int i = 0; i<narr; i++)
    {
        for (int j = i+1; j<narr; j++) //neporovnavame shluk sam se sebou, (i+1)
        {
            akt_vzdalenost = cluster_distance(&carr[i],&carr[j]);
            if (akt_vzdalenost<min_vzdalenost)
            {
                min_vzdalenost = akt_vzdalenost;
                *c1 = i;
                *c2 = j;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
typedef struct
{
    int id;
    char *name;
} Object;

/**
 * Inizializace objektu. Nazev objektu kopiruje. Objekt bude mit nazev
 * roven NULL, pokud se inicializace nezdarila.
 */
Object object_ctor(int id, char *name);

/**
 * Zamena dat dvou objektu.
 */
void object_swap(Object *i1, Object *i2);

/**
 * Hluboka kopie objektu src na dst. Vraci dst, pokud se operace povedla,
 * jinak NULL.
 */
Object *object_cpy(Object *dst, Object *src);

/**
 * Uvolni objekt (resp. jeho jmeno) z pameti.
 */
void object_dtor(Object *o);
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    int id;
    int count = 0;
    float x,y;

    FILE *f = fopen(filename,"r+");

    if (f == NULL)
    {
        fprintf(stderr,"Cannot open file\n");
        arr = NULL;
        return -2;
    }

    if (fscanf(f,"count=%d\n",&count)!=1)
    {
        fprintf(stderr,"Unvalid input: waiting for count=number for first row\n");
        fclose(f);
        return -2;
    }
    if (count<0){
        arr = NULL;
        fprintf(stderr,"A number of clusters should be greater than zero\n");
        fclose(f);
        return -2;
    }
    *arr = malloc(sizeof(struct cluster_t) * count); //alokuju prostoj pro pole vsech shluku
    if (arr == NULL)
    {
        fprintf(stderr,"We didnt get enough memory\n");
        fclose(f);
        return -1;

    }
    for (int i = 0; i< count; i++)
    {
        if (fscanf(f,"%d %f %f",&id,&x,&y)!=3)
        {
            fprintf(stderr,"Bad format, cannot read from file\n");

            for (int idx = i-1; idx>=0; idx--) //ten aktualni spatny jsme jeste nenacetli
            {
                remove_cluster(*arr,i,idx);
            }

            fclose(f);
            return -1;
        }

        /*if (id<0)
        {
            fprintf(stderr,"ID is less than zero\n");
            for (int idx = i-1; idx>=0; idx--) //ten aktualni spatny jsme jeste nenacetli
            {
                remove_cluster(*arr,i,idx);
            }

            fclose(f);
            return -1;
        }*/
        if (x<0||x>1000||y<0||y>1000)
        {
            fprintf(stderr,"x or y is not in correct range\n");
            for (int idx = i-1; idx>=0; idx--) //ten aktualni spatny jsme jeste nenacetli
            {
                remove_cluster(*arr,i,idx);
            }

            fclose(f);
            return -1;
        }
        struct obj_t obj;
        obj.x = x;
        obj.y = y;
        obj.id = id;

        struct cluster_t tmp;
        init_cluster(&tmp,1);
        append_cluster(&tmp, obj);
        arr[0][i] = tmp;
    }
    fclose(f);
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    int cilovy_pocet_shluku = 1;
    char *zbytek;
    int aktualni_poc_shluku = 0;

    //indexy dvou nejblizsich shluku
    int in1 = 0;
    int in2 = 0;

    if (argc>1&&argc<4)
    {
        struct cluster_t *clusters;

        if (argc == 3)
        {
            cilovy_pocet_shluku = strtol(argv[2],&zbytek,10);
            if (cilovy_pocet_shluku<=0)
            {
                fprintf(stderr,"A number of clusters should be greater than zero\n");
                return EXIT_FAILURE;
            }

        }
        aktualni_poc_shluku = load_clusters(argv[1],&clusters);
        if (clusters == NULL)
        {
            fprintf(stderr,"while loading a file, mistake occured\n");
            return EXIT_FAILURE;
        }

        if (aktualni_poc_shluku == -1)
        {
            free(clusters);
            return EXIT_FAILURE;
        }
        if (aktualni_poc_shluku == -2)
        {
            clusters = NULL;
            return EXIT_FAILURE; //nic jsme nemallokovali
        }

        if (cilovy_pocet_shluku>aktualni_poc_shluku)
        {
            cilovy_pocet_shluku = aktualni_poc_shluku;
        }

        while(aktualni_poc_shluku != cilovy_pocet_shluku)
        {
            find_neighbours(clusters,aktualni_poc_shluku,&in1,&in2);
            merge_clusters(&clusters[in1],&clusters[in2]);
            aktualni_poc_shluku = remove_cluster(clusters,aktualni_poc_shluku,in2);
        }
        print_clusters(clusters,aktualni_poc_shluku);
        for (int in = aktualni_poc_shluku-1; in>=0; in--) //vymazu vsechny clustery
        {
            remove_cluster(clusters,aktualni_poc_shluku,in);
        }

        free(clusters);
    }
    return EXIT_SUCCESS;
}
