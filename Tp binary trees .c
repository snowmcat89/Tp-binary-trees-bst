
  /**--------------------------------------------------------**/
  /**       C o n v e r s i o n   Z vers C (Standard)        **/
  /**             Realisee par Pr D.E ZEGOUR                 **/
  /**             E S I - Alger                              **/
  /**             Copywrite 2014                             **/
  /**--------------------------------------------------------**/

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>

  typedef int bool ;
  typedef char * string255 ;

  #define True 1
  #define False 0

  /** Implementation **\: FICHIER **/

  /* Traitement des fichiers ouverts */

  struct _Noeud
    {
      FILE * Var_fich ;
      char * Nom_fich ;
      int Sauv_pos;
      struct _Noeud *Suiv ;
    } ;

  typedef struct _Noeud * _Ptr_Noeud;

  _Ptr_Noeud  _Pile_ouverts  = NULL;

  /* Teste si un fichier est ouvert */
  _Ptr_Noeud _Ouvert ( char * Fp)
    {
      _Ptr_Noeud P;
      bool Trouv ;
      P = _Pile_ouverts; Trouv = False ;
      while ((P != NULL) && ! Trouv )
        if ( strcmp(P->Nom_fich, Fp) == 0)
        Trouv = True;
        else P = P->Suiv;
      return P;
    }

  /* Ajouter un fichier ouvert */
  void _Empiler_ouvert ( char *Fp, FILE *Fl)
    {
      _Ptr_Noeud  P ;
      P = (_Ptr_Noeud) malloc( sizeof( struct _Noeud)) ;
      P->Nom_fich = Fp;
      P->Var_fich = Fl;
      P->Suiv = _Pile_ouverts;
      _Pile_ouverts = P;
    }

  /* Supprimer un fichier ouvert et rendre son nom*/
  char * _Depiler_ouvert ( FILE *Fl)
    {
      char * Fp = malloc (100);
      _Ptr_Noeud P,  Prec  ;
      P= _Pile_ouverts;
      Prec = NULL;
      while (P->Var_fich != Fl )
        { Prec = P ; P = P->Suiv ;}
      strcpy(Fp, P->Nom_fich);
      if (Prec != NULL)
        Prec->Suiv = P->Suiv;
      else _Pile_ouverts = P->Suiv;
      free (P);
      return Fp ;
    }


  /** Fichiers **/

  typedef char _Tx[255];
  /** Types des champs du bloc **/
  typedef string255 Typechamp1_ss;
  typedef _Tx Typechamp1_ss_Buf ;
  typedef string255 Typechamp2_ss;
  typedef _Tx Typechamp2_ss_Buf ;

  /** Type du bloc de donnees du fichier **/
  typedef struct
    {
      Typechamp1_ss_Buf Champ1 ;
      Typechamp2_ss_Buf Champ2 ;
    }  Typestruct1_ss_Buf;

  /** Type de la structure du bloc de donnees du fichier **/
  typedef struct
    {
      Typechamp1_ss Champ1 ;
      Typechamp2_ss Champ2 ;
    }  Typestruct1_ss_;

  typedef Typestruct1_ss_  * Typestruct1_ss ;

  typedef Typestruct1_ss Typestr_Tss;
  typedef Typestruct1_ss_ Type_Tss;


  Typechamp1_ss Struct1_Tss ( Typestruct1_ss Buf )
    {
      return Buf->Champ1;
    }

  Typechamp2_ss Struct2_Tss ( Typestruct1_ss Buf )
    {
      return Buf->Champ2;
    }

  /** Manipulation de la structure **/
  void Aff_struct1_Tss ( Typestruct1_ss Buf, Typechamp1_ss Val )
    {
     strcpy( Buf->Champ1 , Val );
    }

  void Aff_struct2_Tss ( Typestruct1_ss Buf, Typechamp2_ss Val )
    {
     strcpy( Buf->Champ2 , Val );
    }

  /** Machine abstaite sur les fichiers **/

  void Ouvrir_ss ( FILE **ss , char *Fp , char * Mode )
    {
      _Ptr_Noeud P = _Ouvert(Fp);
      if ( P != NULL )
      /* Le fichier est deja ouvert */
        {
         P->Sauv_pos = ftell (P->Var_fich);
         fclose(P->Var_fich);
        }
      /* Le fichier est non ouvert */
      if ( strcmp(Mode,"A") == 0)
        *ss = fopen(Fp, "r+b");
      else
        *ss = fopen(Fp, "w+b");
      _Empiler_ouvert( Fp, *ss);
    }

  void Fermer_ss ( FILE * ss )
    {
      char * Fp = malloc(100);
      _Ptr_Noeud P;
      strcpy(Fp, _Depiler_ouvert(ss));
      fclose(ss) ;
      /* Ya-til un fichier ouvert avec le m?me nom ?  */
      /* Si Oui, le Reouvrir a la position sauvegardee */
      P =  _Ouvert (Fp);
      if ( P != NULL)
      {
        ss = fopen(P->Nom_fich, "r+b");
        fseek(ss, P->Sauv_pos, 0);
      }
    }

  void Ecrireseq_ss ( FILE * ss, Typestruct1_ss Buf  )
    {
      Typestruct1_ss_Buf Buffer ;
      int I, J;
      for(J=0; J<= strlen(Buf->Champ1); ++J)
        Buffer.Champ1[J] = Buf->Champ1[J];
      for(J=0; J<= strlen(Buf->Champ2); ++J)
        Buffer.Champ2[J] = Buf->Champ2[J];
      fwrite(&Buffer, sizeof( Typestruct1_ss_Buf), 1, ss)  ;
    }

  void Ecriredir_ss ( FILE * ss, Typestruct1_ss Buf, int N )
    {
      Typestruct1_ss_Buf Buffer ;
      int I, J;
      for(J=0; J<= strlen(Buf->Champ1); ++J)
        Buffer.Champ1[J] = Buf->Champ1[J];
      for(J=0; J<= strlen(Buf->Champ2); ++J)
        Buffer.Champ2[J] = Buf->Champ2[J];
      fseek(ss, (long) (N-1)* sizeof(  Typestruct1_ss_Buf), 0);
      fwrite(&Buffer, sizeof( Typestruct1_ss_Buf), 1, ss)  ;
    }

  void Lireseq_ss ( FILE * ss, Typestruct1_ss Buf )
    {
      Typestruct1_ss_Buf Buffer ;
      int I, J;
      if (fread(&Buffer, sizeof( Typestruct1_ss_Buf), 1, ss) != 0) {
      for(J=0; J<= strlen(Buffer.Champ1); ++J)
        Buf->Champ1[J] = Buffer.Champ1[J];
      for(J=0; J<= strlen(Buffer.Champ2); ++J)
        Buf->Champ2[J] = Buffer.Champ2[J];
      }
    }

  void Liredir_ss ( FILE * ss, Typestruct1_ss Buf, int N)
    {
      Typestruct1_ss_Buf Buffer ;
      int I, J;
      fseek(ss, (long) (N-1)* sizeof( Typestruct1_ss_Buf), 0 );
      fread(&Buffer, sizeof( Typestruct1_ss_Buf), 1, ss);
      for(J=0; J<= strlen(Buffer.Champ1); ++J)
        Buf->Champ1[J] = Buffer.Champ1[J];
      for(J=0; J<= strlen(Buffer.Champ2); ++J)
        Buf->Champ2[J] = Buffer.Champ2[J];
    }

  void Rajouter_ss ( FILE * ss, Typestruct1_ss Buf )
    {
      Typestruct1_ss_Buf Buffer ;
      int I, J;
      for(J=0; J<= strlen(Buf->Champ1); ++J)
        Buffer.Champ1[J] = Buf->Champ1[J];
      for(J=0; J<= strlen(Buf->Champ2); ++J)
        Buffer.Champ2[J] = Buf->Champ2[J];
      fseek(ss, 0, 2); /* Fin du fichier */
      fwrite(&Buffer, sizeof( Typestruct1_ss_Buf), 1, ss)  ;
    }

  bool Finfich_ss (FILE * ss)
    {
      long K = ftell(ss);
      fseek(ss, 0, 2); /* Fin du fichier */
      long K2 = ftell(ss);   /* position a partir du debut */
      if  (K==K2)
        { fseek(ss, K, 0); return 1;}
      else
        { fseek(ss, K, 0); return 0;}
    }

  int Alloc_bloc_ss (FILE * ss)
    {
      long K;
      fseek(ss, 0, 2); /* Fin du fichier */
      K = ftell(ss);   /* position a partir du debut */
      K = K / sizeof (Typestruct1_ss_Buf);
      K ++;
      return(K);
    }


  /** Implementation **\: ARBRE BINAIRE DE CHAINES DE CARACTERES**/

  /** Arbres de recherche binaire **/

  typedef string255 Typeelem_As   ;
  typedef struct Noeud_As * Pointeur_As ;

  struct Noeud_As
    {
      Typeelem_As  Val ;
      Pointeur_As Fg ;
      Pointeur_As Fd ;
      Pointeur_As Pere ;
     } ;

  Typeelem_As Info_As( Pointeur_As P )
    { return P->Val;   }

  Pointeur_As Fg_As( Pointeur_As P)
    { return P->Fg ; }

  Pointeur_As Fd_As( Pointeur_As P)
    { return P->Fd ; }

  Pointeur_As Pere_As( Pointeur_As P)
    { return P->Pere ; }

  void Aff_info_As ( Pointeur_As P, Typeelem_As Val)
    {
       strcpy( P->Val , Val );
    }

  void Aff_fg_As( Pointeur_As P, Pointeur_As Q)
    { P->Fg =  Q;  }

  void Aff_fd_As( Pointeur_As P, Pointeur_As Q)
    { P->Fd =  Q ; }

  void Aff_pere_As( Pointeur_As P, Pointeur_As Q)
    { P->Pere =  Q ; }

  void Creernoeud_As( Pointeur_As *P)
    {
      *P = (struct Noeud_As *) malloc( sizeof( struct Noeud_As))   ;
      (*P)->Val = malloc(255 * sizeof(string255));
      (*P)->Fg = NULL;
      (*P)->Fd = NULL;
      (*P)->Pere = NULL;
    }

  void Liberernoeud_As( Pointeur_As P)
    { free( P ) ; }


  /** Implementation **\: FILE DE ARBRES BINAIRES DE CHAINES DE CARACTERES**/
  /** Files d'attente **/

  typedef Pointeur_As Typeelem_FAs ;
  typedef  struct Filedattente_FAs * Pointeur_FAs ;
  typedef  struct Maillon_FAs * Ptliste_FAs ;

  struct Maillon_FAs
    {
      Typeelem_FAs Val ;
      Ptliste_FAs Suiv  ;
    };

  struct Filedattente_FAs
    {
      Ptliste_FAs Tete, Queue ;
    };

  void Creerfile_FAs ( Pointeur_FAs *Fil   )
    {
      *Fil = (struct Filedattente_FAs *) malloc( sizeof( struct Filedattente_FAs)) ;
      (*Fil)->Tete = NULL ;
      (*Fil)->Queue = NULL ;
    }

  bool Filevide_FAs (Pointeur_FAs Fil  )
    { return  Fil->Tete == NULL ;}

  void Enfiler_FAs ( Pointeur_FAs Fil , Typeelem_FAs Val   )
    {
      Ptliste_FAs Q;

      Q = (struct Maillon_FAs *) malloc( sizeof( struct Maillon_FAs))   ;
      Q->Val = Val ;
      Q->Suiv = NULL;
      if ( ! Filevide_FAs(Fil) )
        Fil->Queue->Suiv = Q ;
      else Fil->Tete = Q;
        Fil->Queue = Q;
    }

  void Defiler_FAs (Pointeur_FAs Fil, Typeelem_FAs *Val )
    {
      if (! Filevide_FAs(Fil) )
        {
          *Val = Fil->Tete->Val ;
          Fil->Tete =  Fil->Tete->Suiv;
        }
      else printf ("%s \n", "File d'attente vide");
    }

  /** Pour les variables temporaires **/
  typedef string255 Typeelem_V1s;
  typedef Typeelem_V1s Typetab_V1s[1];


  /** Implementation **\: PILE DE ARBRES BINAIRES DE CHAINES DE CARACTERES**/
  /** Piles **/

  typedef Pointeur_As Typeelem_PAs ;
  typedef struct Maillon_PAs * Pointeur_PAs ;
  typedef   Pointeur_PAs  Typepile_PAs  ;

  struct Maillon_PAs
    {
      Typeelem_PAs  Val ;
      Pointeur_PAs Suiv ;
    } ;

  void Creerpile_PAs( Pointeur_PAs *P )
    { *P = NULL ; }

  bool Pilevide_PAs ( Pointeur_PAs P )
    { return  (P == NULL ); }

  void Empiler_PAs ( Pointeur_PAs *P,  Typeelem_PAs Val )
    {
      Pointeur_PAs Q;

      Q = (struct Maillon_PAs *) malloc( sizeof( struct Maillon_PAs))   ;
      Q->Val = Val ;
      Q->Suiv = *P;
      *P = Q;
    }

  void Depiler_PAs ( Pointeur_PAs *P,  Typeelem_PAs *Val )
    {
      Pointeur_PAs Sauv;

      if (! Pilevide_PAs (*P) )
        {
          *Val = (*P)->Val ;
          Sauv = *P;
          *P = (*P)->Suiv;
          free(Sauv);
        }
      else printf ("%s \n", "Pile vide");
    }


  /** Variables du programme principal **/
  FILE *F;
  Typestruct1_ss S ;
  int I;
  int N;
  Pointeur_As Bst1=NULL;
  Pointeur_As P=NULL;
  Pointeur_As Pr=NULL;
  string255 Mot;
  bool Possible;
  Pointeur_FAs G=NULL;
  Pointeur_As M=NULL;
  int Random;
  Pointeur_As Bst2=NULL;
  Pointeur_As Bst3=NULL;
  string255 Word1;
  string255 Word2;
  string255 Le_mot;
  int Choice;

  /** Fonctions standards **/

  char *Caract ( string255 Ch , int I )
    {
      char *s = malloc(2);
      s[0] = Ch[I-1];
      s[1] = '\0';
      return  s;
    }

  int Aleanombre( int N )
    { return ( rand() % N ); }

  char  *Aleachaine ( int N )
    {
      int k;
      char  * Chaine = malloc(N+1);

      char Chr1[26] = "abcdefghijklmnopqrstuvwxyz";
      char Chr2[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

      for (k=0;k<N; k++)
        switch ( rand() % 2 ){
        case 0 :  *(Chaine+k) = Chr1[rand() % 26] ; break ;
        case 1 :  *(Chaine+k) = Chr2[rand() % 26] ; break ;
        }
      Chaine[k] =  '\0' ;

      return (Chaine);
    }

  int Max (int a, int b)
    {
      if (a > b) return(a);
      else return(b);
    }


  /** Creation d'un arbre de recherche binaire **/

  void Creer_arb_As ( Pointeur_As *Arbre, Typetab_V1s Tab, int  N)
    {
      int I;
      Pointeur_As P, Q ;

      /* Creation De La Racine */
      Creernoeud_As ( &P ) ;
      *Arbre = P ;
      Aff_info_As ( P , Tab[0] ) ;
      for(I=2;I<=N;++I)
        {
          P = *Arbre ;
          Q = *Arbre ;
          while (( strcmp(Tab[I-1] , Info_As ( P ))!=0 ) && ( Q != NULL ))
            {
              P = Q ;
              if (strcmp( Tab[I-1] , Info_As ( P ))< 0 )
                Q = Fg_As ( P );
              else Q = Fd_As ( P ) ;
            }
          if (strcmp( Tab[I-1] , Info_As ( P ))==0 )
            printf ("%s \n", "Un double existe");
          else
            {
              Creernoeud_As ( &Q ) ;
              Aff_info_As ( Q , Tab[I-1] ) ;
              if (strcmp( Tab[I-1] , Info_As ( P ))< 0 )
                Aff_fg_As ( P , Q ) ;
              else Aff_fd_As ( P , Q );
              Aff_pere_As (Q, P);
            }

        }
    }


  /** Initialisation d'une structure **/
  void Init_struct_Tss ( Typestr_Tss S, Type_Tss S_ )
    {
      S->Champ1 = S_.Champ1 ;
      S->Champ2 = S_.Champ2 ;
    }


  /** Prototypes des fonctions **/

  void Menu_principale();
  void Creer_arb1 (Pointeur_As *R , string255 *Mot , Pointeur_As *Rg);
  void Fillfichier (FILE *F , int *N);
  void Rotation_bst2 (Pointeur_As *Noeud_principale , Pointeur_As *Racine_globale);
  void Pivot_droit_bst2 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale);
  void Pivot_gauche_bst2 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale);
  void Creer_arb2 (Pointeur_As *R , string255 *Mot , Pointeur_As *Rg);
  void Monter_racine_bst1 (Pointeur_As *Noeud_principale , Pointeur_As *Racine_globale);
  void Pivot_droit_bst1 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale);
  void Pivot_gauche_bst1 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale);
  bool  Rech_bst1 (Pointeur_As *Racine , string255 *Val) ;
  bool  Rech_bst2 (Pointeur_As *Racine , string255 *Val) ;
  bool  Rech_bst3 (Pointeur_As *Racine , string255 *Val) ;
  int  Profondeur (Pointeur_As *Racine) ;
  int  Length (Pointeur_As *Racine , Pointeur_As *Maillon) ;
  void Creer_arb3 (Pointeur_As *R , string255 *Mot , Pointeur_As *Rg);
  void Monter_racine_bst3 (Pointeur_As *Noeud_principale , Pointeur_As *Racine_globale);
  void Pivot_droit_bst3 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale);
  void Pivot_gauche_bst3 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale);
  string255  Trouve_min_sup (Pointeur_As *Racine , string255 *Word) ;
  string255  Trouve_max_inf (Pointeur_As *Racine , string255 *Word) ;
  void Print_words_in_range (Pointeur_As *Racine , string255 *Word1 , string255 *Word2);
  void Parcours_infixe_range (Pointeur_As *Current , string255 *Lower , string255 *Upper);
  int  Compte_yza (Pointeur_As *Racine) ;
  void Inordre (Pointeur_As *Racine);
  void Xyz_level (Pointeur_As *Root);

  void Menu_principale()
    {

      /** Corps du module **/
     printf ( " %s", "=========================================================" ) ;
     printf ( " %s", "=                PROJET RÉALISÉ PAR :                   " ) ;
     printf ( " %s", "=          GUENDOUZ FAHD & SID MOHAND AMAR              " ) ;
     printf ( " %s", "=========================================================" ) ;
     printf ( " %s", "=========================================================" ) ;
     printf ( " %s", "=                      MENU PRINCIPAL                   " ) ;
     printf ( " %s", "=========================================================" ) ;
     printf ( " %s", "=  1/ Compter les mots commençant par Y, Z ou X         " ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  2/ Calculer la profondeur de l''arbre BST01          " ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  3/ Calculer la profondeur de l''arbre BST02          " ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  4/ Calculer la profondeur de l''arbre BST03          " ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  5/ Parcours inordre des trois arbres                 " ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  6/ Nombre de nœuds commençant par Y,Z ou X par niveau" ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  7/ Rechercher un mot dans un des arbres              " ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  8/ Rechercher des mots dans l''intervalle [A,B]      " ) ;
     printf ( " %s", "=                                                       " ) ;
     printf ( " %s", "=  9/ QUITTER                                           " ) ;
     printf ( " %s", "=========================================================" ) ;
     printf ( " %s", "" ) ;
     printf ( " %s", "Veuillez saisir votre choix (1-9) : " ) ;

    }
  void Creer_arb1 (Pointeur_As *R , string255 *Mot , Pointeur_As *Rg)
    {
      /** Variables locales **/
      Pointeur_As Racine=NULL;
      Pointeur_As Tree=NULL;
      string255 X;
      string255 Y;
      string255 Z;
        Typetab_V1s T_Rg;
        Typetab_V1s T_Tree;
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     X = malloc(255 * sizeof(char));
     Y = malloc(255 * sizeof(char));
     Z = malloc(255 * sizeof(char));
     int _Izw2;for (_Izw2=0; _Izw2<1; ++_Izw2)
       T_Rg[_Izw2]= malloc(255 * sizeof(char));
     int _Izw3;for (_Izw3=0; _Izw3<1; ++_Izw3)
       T_Tree[_Izw3]= malloc(255 * sizeof(char));
     Racine  =  *R ;
     strcpy (X,   "Y") ;
     strcpy (Y,   "Z") ;
     strcpy (Z,   "a") ;
     if( ( Racine == NULL ))   {
       strcpy(T_Rg [ 0 ] , *Mot );
       Creer_arb_As (&*Rg , T_Rg , 1 )  ;
       Aff_pere_As ( *Rg , NULL ) ;
       }
     else
       {
       if(strcmp( Info_As ( Racine ), *Mot) > 0 )   {
         if( Fg_As ( Racine ) == NULL)   {
           strcpy(T_Tree [ 0 ] , *Mot );
           Creer_arb_As (&Tree , T_Tree , 1 )  ;
           Aff_pere_As ( Tree , Racine ) ;
           Aff_fg_As ( Racine , Tree ) ;
           if( (strcmp( Caract ( *Mot , 1 ), X) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Y) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Z) == 0  ))   {
             Monter_racine_bst1 ( & Tree , & *Rg ) ;

           } ;
           }
         else
           {
           _Px1 =  Fg_As ( Racine ) ;
           Creer_arb1 ( &_Px1, & *Mot , & *Rg ) ;

         } ;
         }
       else
         {
         if(strcmp( Info_As ( Racine ), *Mot) < 0 )   {
           if( Fd_As ( Racine ) == NULL)   {
             strcpy(T_Tree [ 0 ] , *Mot );
             Creer_arb_As (&Tree , T_Tree , 1 )  ;
             Aff_pere_As ( Tree , Racine ) ;
             Aff_fd_As ( Racine , Tree ) ;
             if( (strcmp( Caract ( *Mot , 1 ), X) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Y) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Z) == 0  ))   {
               Monter_racine_bst1 ( & Tree , & *Rg ) ;

             } ;
             }
           else
             {
             _Px2 =  Fd_As ( Racine ) ;
             Creer_arb1 ( &_Px2, & *Mot , & *Rg ) ;

           } ;

         } ;

       } ;

     } ;

    }
  void Fillfichier (FILE *F , int *N)
    {
      /** Variables locales **/
      Typestruct1_ss S ;
      int I;
      string255 _Sx;
      Type_Tss S_S;

      /** Corps du module **/
     S = malloc(sizeof(Typestruct1_ss));
     S->Champ1 = malloc(255 * sizeof(char));
     S->Champ2 = malloc(255 * sizeof(char));
     _Sx = malloc(255 * sizeof(char));
     Ouvrir_ss (&F , "F2.z" , "N" ) ;
     for( I  =  1 ;I <=  100 ; ++I){
       Random  =  Aleanombre(5 ) ;
       if( Random == 4)   {
         strcpy (_Sx, "Y");
         S_S.Champ1= malloc(255 * sizeof(char));
         strcpy(S_S.Champ1 , strcat(_Sx,Aleachaine(5))) ;
         S_S.Champ2= malloc(255 * sizeof(char));
         strcpy(S_S.Champ2 , "") ;
         Init_struct_Tss ( S , S_S )  ;
         }
       else
         {
         if( Random == 3)   {
           strcpy (_Sx, "Z");
           S_S.Champ1= malloc(255 * sizeof(char));
           strcpy(S_S.Champ1 , strcat(_Sx,Aleachaine(5))) ;
           S_S.Champ2= malloc(255 * sizeof(char));
           strcpy(S_S.Champ2 , "") ;
           Init_struct_Tss ( S , S_S )  ;
           }
         else
           {
           if( Random == 3)   {
             strcpy (_Sx, "a");
             S_S.Champ1= malloc(255 * sizeof(char));
             strcpy(S_S.Champ1 , strcat(_Sx,Aleachaine(5))) ;
             S_S.Champ2= malloc(255 * sizeof(char));
             strcpy(S_S.Champ2 , "") ;
             Init_struct_Tss ( S , S_S )  ;
             }
           else
             {
             S_S.Champ1= malloc(255 * sizeof(char));
             strcpy(S_S.Champ1 , Aleachaine(5)) ;
             S_S.Champ2= malloc(255 * sizeof(char));
             strcpy(S_S.Champ2 , "") ;
             Init_struct_Tss ( S , S_S )  ;

           } ;

         } ;

       } ;
       Ecrireseq_ss ( F , S );
     } ;
     Fermer_ss ( F ) ;
     Ouvrir_ss (&F , "F2.z" , "A" ) ;
     I  =  0 ;
     while( ! Finfich_ss ( F )) {
       Lireseq_ss ( F , S ) ;
       strcpy (Mot,   Struct1_Tss ( S  )) ;
       Creer_arb1 ( & Bst1 , & Mot , & Bst1 ) ;
       Creer_arb2 ( & Bst2 , & Mot , & Bst2 ) ;
       Creer_arb3 ( & Bst3 , & Mot , & Bst3 ) ;
       I  =  I + 1;
 } ;
     Fermer_ss ( F ) ;

    }
  void Rotation_bst2 (Pointeur_As *Noeud_principale , Pointeur_As *Racine_globale)
    {
      /** Variables locales **/
      int I;
      int Lengt;
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     Lengt  =  ( ( Length ( & *Racine_globale , & *Noeud_principale ) ) / 2 ) ;
     for( I  =  1 ;I <=  Lengt ; ++I) {
       if( Fg_As ( Pere_As ( *Noeud_principale ) ) == *Noeud_principale) {
         ;
         _Px1 =  Pere_As ( *Noeud_principale ) ;
         Pivot_droit_bst2 ( &_Px1, & *Racine_globale ) ;
         }
       else
         {
         _Px2 =  Pere_As ( *Noeud_principale ) ;
         Pivot_gauche_bst2 ( &_Px2, & *Racine_globale ) ;

       } ;

     } ;

    }
  void Pivot_droit_bst2 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale)
    {
      /** Variables locales **/
      Pointeur_As Enfant_g=NULL;
      Pointeur_As Gp=NULL;
      Pointeur_As Sous_arbre_d=NULL;

      /** Corps du module **/
     Enfant_g  =  Fg_As ( *Noeud_parent ) ;
     Sous_arbre_d  =  Fd_As ( Enfant_g ) ;
     Gp  =  Pere_As ( *Noeud_parent ) ;
     Aff_fd_As ( Enfant_g , *Noeud_parent ) ;
     Aff_fg_As ( *Noeud_parent , Sous_arbre_d ) ;
     if( Sous_arbre_d != NULL) {
       Aff_pere_As ( Sous_arbre_d , *Noeud_parent ) ;

     } ;
     Aff_pere_As ( Enfant_g , Gp ) ;
     Aff_pere_As ( *Noeud_parent , Enfant_g ) ;
     if( Gp == NULL) {
       *Racine_originale  =  Enfant_g ;
       }
     else
       {
       if( Fg_As ( Gp ) == *Noeud_parent) {
         Aff_fg_As ( Gp , Enfant_g ) ;
         }
       else
         {
         Aff_fd_As ( Gp , Enfant_g ) ;

       } ;

     } ;

    }
  void Pivot_gauche_bst2 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale)
    {
      /** Variables locales **/
      Pointeur_As Enfant_d=NULL;
      Pointeur_As Gp=NULL;
      Pointeur_As Sous_arbre_g=NULL;

      /** Corps du module **/
     Enfant_d  =  Fd_As ( *Noeud_parent ) ;
     Sous_arbre_g  =  Fg_As ( Enfant_d ) ;
     Gp  =  Pere_As ( *Noeud_parent ) ;
     Aff_fg_As ( Enfant_d , *Noeud_parent ) ;
     Aff_fd_As ( *Noeud_parent , Sous_arbre_g ) ;
     if( Sous_arbre_g != NULL) {
       Aff_pere_As ( Sous_arbre_g , *Noeud_parent ) ;

     } ;
     Aff_pere_As ( Enfant_d , Gp ) ;
     Aff_pere_As ( *Noeud_parent , Enfant_d ) ;
     if( Gp == NULL) {
       *Racine_originale  =  Enfant_d ;
       }
     else
       {
       if( Fg_As ( Gp ) == *Noeud_parent) {
         Aff_fg_As ( Gp , Enfant_d ) ;
         }
       else
         {
         Aff_fd_As ( Gp , Enfant_d ) ;

       } ;

     } ;

    }
  void Creer_arb2 (Pointeur_As *R , string255 *Mot , Pointeur_As *Rg)
    {
      /** Variables locales **/
      Pointeur_As Racine=NULL;
      Pointeur_As Tree=NULL;
      string255 X;
      string255 Y;
      string255 Z;
        Typetab_V1s T_Rg;
        Typetab_V1s T_Tree;
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     X = malloc(255 * sizeof(char));
     Y = malloc(255 * sizeof(char));
     Z = malloc(255 * sizeof(char));
     int _Izw2;for (_Izw2=0; _Izw2<1; ++_Izw2)
       T_Rg[_Izw2]= malloc(255 * sizeof(char));
     int _Izw3;for (_Izw3=0; _Izw3<1; ++_Izw3)
       T_Tree[_Izw3]= malloc(255 * sizeof(char));
     Racine  =  *R ;
     strcpy (X,   "Y") ;
     strcpy (Y,   "Z") ;
     strcpy (Z,   "a") ;
     if( ( Racine == NULL ))   {
       strcpy(T_Rg [ 0 ] , *Mot );
       Creer_arb_As (&*Rg , T_Rg , 1 )  ;
       Aff_pere_As ( *Rg , NULL ) ;
       }
     else
       {
       if(strcmp( Info_As ( Racine ), *Mot) > 0 )   {
         if( Fg_As ( Racine ) == NULL)   {
           strcpy(T_Tree [ 0 ] , *Mot );
           Creer_arb_As (&Tree , T_Tree , 1 )  ;
           Aff_pere_As ( Tree , Racine ) ;
           Aff_fg_As ( Racine , Tree ) ;
           if( (strcmp( Caract ( *Mot , 1 ), X) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Y) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Z) == 0  ))   {
             Rotation_bst2 ( & Tree , & *Rg ) ;

           } ;
           }
         else
           {
           _Px1 =  Fg_As ( Racine ) ;
           Creer_arb2 ( &_Px1, & *Mot , & *Rg ) ;

         } ;
         }
       else
         {
         if(strcmp( Info_As ( Racine ), *Mot) < 0 )   {
           if( Fd_As ( Racine ) == NULL)   {
             strcpy(T_Tree [ 0 ] , *Mot );
             Creer_arb_As (&Tree , T_Tree , 1 )  ;
             Aff_pere_As ( Tree , Racine ) ;
             Aff_fd_As ( Racine , Tree ) ;
             if( (strcmp( Caract ( *Mot , 1 ), X) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Y) == 0  ) || (strcmp( Caract ( *Mot , 1 ), Z) == 0  ))   {
               Rotation_bst2 ( & Tree , & *Rg ) ;

             } ;
             }
           else
             {
             _Px2 =  Fd_As ( Racine ) ;
             Creer_arb2 ( &_Px2, & *Mot , & *Rg ) ;

           } ;

         } ;

       } ;

     } ;

    }
  void Monter_racine_bst1 (Pointeur_As *Noeud_principale , Pointeur_As *Racine_globale)
    {
      /** Variables locales **/
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     while( Pere_As ( *Noeud_principale ) != NULL)  {
       if( Fg_As ( Pere_As ( *Noeud_principale ) ) == *Noeud_principale) {
         ;
         _Px1 =  Pere_As ( *Noeud_principale ) ;
         Pivot_droit_bst1 ( &_Px1, & *Racine_globale ) ;
         }
       else
         {
         _Px2 =  Pere_As ( *Noeud_principale ) ;
         Pivot_gauche_bst1 ( &_Px2, & *Racine_globale ) ;

       } ;

 } ;

    }
  void Pivot_droit_bst1 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale)
    {
      /** Variables locales **/
      Pointeur_As Enfant_g=NULL;
      Pointeur_As Gp=NULL;
      Pointeur_As Sous_arbre_d=NULL;

      /** Corps du module **/
     Enfant_g  =  Fg_As ( *Noeud_parent ) ;
     Sous_arbre_d  =  Fd_As ( Enfant_g ) ;
     Gp  =  Pere_As ( *Noeud_parent ) ;
     Aff_fd_As ( Enfant_g , *Noeud_parent ) ;
     Aff_fg_As ( *Noeud_parent , Sous_arbre_d ) ;
     if( Sous_arbre_d != NULL) {
       Aff_pere_As ( Sous_arbre_d , *Noeud_parent ) ;

     } ;
     Aff_pere_As ( Enfant_g , Gp ) ;
     Aff_pere_As ( *Noeud_parent , Enfant_g ) ;
     if( Gp == NULL) {
       *Racine_originale  =  Enfant_g ;
       }
     else
       {
       if( Fg_As ( Gp ) == *Noeud_parent) {
         Aff_fg_As ( Gp , Enfant_g ) ;
         }
       else
         {
         Aff_fd_As ( Gp , Enfant_g ) ;

       } ;

     } ;

    }
  void Pivot_gauche_bst1 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale)
    {
      /** Variables locales **/
      Pointeur_As Enfant_d=NULL;
      Pointeur_As Gp=NULL;
      Pointeur_As Sous_arbre_g=NULL;

      /** Corps du module **/
     Enfant_d  =  Fd_As ( *Noeud_parent ) ;
     Sous_arbre_g  =  Fg_As ( Enfant_d ) ;
     Gp  =  Pere_As ( *Noeud_parent ) ;
     Aff_fg_As ( Enfant_d , *Noeud_parent ) ;
     Aff_fd_As ( *Noeud_parent , Sous_arbre_g ) ;
     if( Sous_arbre_g != NULL) {
       Aff_pere_As ( Sous_arbre_g , *Noeud_parent ) ;

     } ;
     Aff_pere_As ( Enfant_d , Gp ) ;
     Aff_pere_As ( *Noeud_parent , Enfant_d ) ;
     if( Gp == NULL) {
       *Racine_originale  =  Enfant_d ;
       }
     else
       {
       if( Fg_As ( Gp ) == *Noeud_parent) {
         Aff_fg_As ( Gp , Enfant_d ) ;
         }
       else
         {
         Aff_fd_As ( Gp , Enfant_d ) ;

       } ;

     } ;

    }
  bool  Rech_bst1 (Pointeur_As *Racine , string255 *Val)
    {
      /** Variables locales **/
      bool  Rech_bst12 ;
      Pointeur_As P=NULL;
      bool Trouv;

      /** Corps du module **/
     Trouv  =  False ;
     P  =  *Racine ;
     while( ( P != NULL ) && ( ! Trouv ))  {
       if( (strcmp( Caract ( Info_As ( P ) , 1 ), "Y") == 0  ) || (strcmp( Caract ( Info_As ( P ) , 1 ), "Z") == 0  ) || (strcmp( Caract ( Info_As ( P ) , 1 ), "a") == 0  ))   {
         if(strcmp( *Val, Info_As ( P )) == 0 )   {
           Trouv  =  True ;
           }
         else
           {
           if(strcmp( *Val, Info_As ( P )) < 0 )   {
             P  =  Fg_As ( P ) ;
             }
           else
             {
             P  =  Fd_As ( P ) ;

           } ;

         } ;
         }
       else
         {
         if(strcmp( *Val, Info_As ( P )) < 0 )   {
           P  =  Fg_As ( P ) ;
           }
         else
           {
           P  =  Fd_As ( P ) ;

         } ;

       } ;

 } ;
     Rech_bst12  =  Trouv ;

     return Rech_bst12 ;
    }
  bool  Rech_bst2 (Pointeur_As *Racine , string255 *Val)
    {
      /** Variables locales **/
      bool  Rech_bst22 ;
      Pointeur_As P=NULL;
      bool Trouv;

      /** Corps du module **/
     Trouv  =  False ;
     P  =  *Racine ;
     while( ( P != NULL ) && ( ! Trouv ))  {
       if(strcmp( *Val, Info_As ( P )) == 0 )   {
         Trouv  =  True ;
         }
       else
         {
         if(strcmp( *Val, Info_As ( P )) < 0 )   {
           P  =  Fg_As ( P ) ;
           }
         else
           {
           P  =  Fd_As ( P ) ;

         } ;

       } ;

 } ;
     Rech_bst22  =  Trouv ;

     return Rech_bst22 ;
    }
  bool  Rech_bst3 (Pointeur_As *Racine , string255 *Val)
    {
      /** Variables locales **/
      bool  Rech_bst32 ;
      Pointeur_As P=NULL;
      bool Trouv;

      /** Corps du module **/
     Trouv  =  False ;
     P  =  *Racine ;
     while( ( P != NULL ) && ( ! Trouv ))  {
       if( (strcmp( Caract ( Info_As ( P ) , 1 ), "Y") != 0  ) && (strcmp( Caract ( Info_As ( P ) , 1 ), "Z") != 0  ) && (strcmp( Caract ( Info_As ( P ) , 1 ), "a") != 0  ))   {
         if(strcmp( *Val, Info_As ( P )) == 0 )   {
           Trouv  =  True ;
           }
         else
           {
           if(strcmp( *Val, Info_As ( P )) < 0 )   {
             P  =  Fg_As ( P ) ;
             }
           else
             {
             P  =  Fd_As ( P ) ;

           } ;

         } ;
         }
       else
         {
         if(strcmp( *Val, Info_As ( P )) < 0 )   {
           P  =  Fg_As ( P ) ;
           }
         else
           {
           P  =  Fd_As ( P ) ;

         } ;

       } ;

 } ;
     Rech_bst32  =  Trouv ;

     return Rech_bst32 ;
    }
  int  Profondeur (Pointeur_As *Racine)
    {
      /** Variables locales **/
      int  Profondeur2 ;
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     if( ( *Racine == NULL ))   {
       Profondeur2  =  - 1 ;
       }
     else
       {
       _Px1 =  Fg_As ( *Racine ) ;
       _Px2 =  Fd_As ( *Racine ) ;
       Profondeur2  =  1 + Max ( Profondeur ( &_Px1) , Profondeur ( &_Px2) ) ;

     } ;

     return Profondeur2 ;
    }
  int  Length (Pointeur_As *Racine , Pointeur_As *Maillon)
    {
      /** Variables locales **/
      int  Length2 ;
      int Cpt;
      Pointeur_As Current=NULL;

      /** Corps du module **/
     Cpt  =  - 1 ;
     if( ( *Racine == NULL ) || ( *Maillon == NULL )) {
       Length2  =  0 ;
       }
     else
       {
       Current  =  *Maillon ;
       while( ( Current != *Racine ) && ( Current != NULL )) {
         Cpt  =  Cpt + 1 ;
         Current  =  Pere_As ( Current ) ;

 } ;
       if( ( Current == *Racine )) {
         Cpt  =  Cpt + 1 ;

       } ;

     } ;
     Length2  =  Cpt ;

     return Length2 ;
    }
  void Creer_arb3 (Pointeur_As *R , string255 *Mot , Pointeur_As *Rg)
    {
      /** Variables locales **/
      Pointeur_As Racine=NULL;
      Pointeur_As Tree=NULL;
      string255 X;
      string255 Y;
      string255 Z;
        Typetab_V1s T_Rg;
        Typetab_V1s T_Tree;
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     X = malloc(255 * sizeof(char));
     Y = malloc(255 * sizeof(char));
     Z = malloc(255 * sizeof(char));
     int _Izw2;for (_Izw2=0; _Izw2<1; ++_Izw2)
       T_Rg[_Izw2]= malloc(255 * sizeof(char));
     int _Izw3;for (_Izw3=0; _Izw3<1; ++_Izw3)
       T_Tree[_Izw3]= malloc(255 * sizeof(char));
     Racine  =  *R ;
     strcpy (X,   "Y") ;
     strcpy (Y,   "Z") ;
     strcpy (Z,   "a") ;
     if( ( Racine == NULL ))   {
       strcpy(T_Rg [ 0 ] , *Mot );
       Creer_arb_As (&*Rg , T_Rg , 1 )  ;
       Aff_pere_As ( *Rg , NULL ) ;
       }
     else
       {
       if(strcmp( Info_As ( Racine ), *Mot) > 0 )   {
         if( Fg_As ( Racine ) == NULL)   {
           strcpy(T_Tree [ 0 ] , *Mot );
           Creer_arb_As (&Tree , T_Tree , 1 )  ;
           Aff_pere_As ( Tree , Racine ) ;
           Aff_fg_As ( Racine , Tree ) ;
           if( (strcmp( Caract ( *Mot , 1 ), X) != 0  ) && (strcmp( Caract ( *Mot , 1 ), Y) != 0  ) && (strcmp( Caract ( *Mot , 1 ), Z) != 0  ))   {
             Monter_racine_bst3 ( & Tree , & *Rg ) ;

           } ;
           }
         else
           {
           _Px1 =  Fg_As ( Racine ) ;
           Creer_arb3 ( &_Px1, & *Mot , & *Rg ) ;

         } ;
         }
       else
         {
         if(strcmp( Info_As ( Racine ), *Mot) < 0 )   {
           if( Fd_As ( Racine ) == NULL)   {
             strcpy(T_Tree [ 0 ] , *Mot );
             Creer_arb_As (&Tree , T_Tree , 1 )  ;
             Aff_pere_As ( Tree , Racine ) ;
             Aff_fd_As ( Racine , Tree ) ;
             if( (strcmp( Caract ( *Mot , 1 ), X) != 0  ) && (strcmp( Caract ( *Mot , 1 ), Y) != 0  ) && (strcmp( Caract ( *Mot , 1 ), Z) != 0  ))   {
               Monter_racine_bst3 ( & Tree , & *Rg ) ;

             } ;
             }
           else
             {
             _Px2 =  Fd_As ( Racine ) ;
             Creer_arb3 ( &_Px2, & *Mot , & *Rg ) ;

           } ;

         } ;

       } ;

     } ;

    }
  void Monter_racine_bst3 (Pointeur_As *Noeud_principale , Pointeur_As *Racine_globale)
    {
      /** Variables locales **/
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     while( Pere_As ( *Noeud_principale ) != NULL)  {
       if( Fg_As ( Pere_As ( *Noeud_principale ) ) == *Noeud_principale) {
         ;
         _Px1 =  Pere_As ( *Noeud_principale ) ;
         Pivot_droit_bst3 ( &_Px1, & *Racine_globale ) ;
         }
       else
         {
         _Px2 =  Pere_As ( *Noeud_principale ) ;
         Pivot_gauche_bst3 ( &_Px2, & *Racine_globale ) ;

       } ;

 } ;

    }
  void Pivot_droit_bst3 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale)
    {
      /** Variables locales **/
      Pointeur_As Enfant_g=NULL;
      Pointeur_As Gp=NULL;
      Pointeur_As Sous_arbre_d=NULL;

      /** Corps du module **/
     Enfant_g  =  Fg_As ( *Noeud_parent ) ;
     Sous_arbre_d  =  Fd_As ( Enfant_g ) ;
     Gp  =  Pere_As ( *Noeud_parent ) ;
     Aff_fd_As ( Enfant_g , *Noeud_parent ) ;
     Aff_fg_As ( *Noeud_parent , Sous_arbre_d ) ;
     if( Sous_arbre_d != NULL) {
       Aff_pere_As ( Sous_arbre_d , *Noeud_parent ) ;

     } ;
     Aff_pere_As ( Enfant_g , Gp ) ;
     Aff_pere_As ( *Noeud_parent , Enfant_g ) ;
     if( Gp == NULL) {
       *Racine_originale  =  Enfant_g ;
       }
     else
       {
       if( Fg_As ( Gp ) == *Noeud_parent) {
         Aff_fg_As ( Gp , Enfant_g ) ;
         }
       else
         {
         Aff_fd_As ( Gp , Enfant_g ) ;

       } ;

     } ;

    }
  void Pivot_gauche_bst3 (Pointeur_As *Noeud_parent , Pointeur_As *Racine_originale)
    {
      /** Variables locales **/
      Pointeur_As Enfant_d=NULL;
      Pointeur_As Gp=NULL;
      Pointeur_As Sous_arbre_g=NULL;

      /** Corps du module **/
     Enfant_d  =  Fd_As ( *Noeud_parent ) ;
     Sous_arbre_g  =  Fg_As ( Enfant_d ) ;
     Gp  =  Pere_As ( *Noeud_parent ) ;
     Aff_fg_As ( Enfant_d , *Noeud_parent ) ;
     Aff_fd_As ( *Noeud_parent , Sous_arbre_g ) ;
     if( Sous_arbre_g != NULL) {
       Aff_pere_As ( Sous_arbre_g , *Noeud_parent ) ;

     } ;
     Aff_pere_As ( Enfant_d , Gp ) ;
     Aff_pere_As ( *Noeud_parent , Enfant_d ) ;
     if( Gp == NULL) {
       *Racine_originale  =  Enfant_d ;
       }
     else
       {
       if( Fg_As ( Gp ) == *Noeud_parent) {
         Aff_fg_As ( Gp , Enfant_d ) ;
         }
       else
         {
         Aff_fd_As ( Gp , Enfant_d ) ;

       } ;

     } ;

    }
  string255  Trouve_min_sup (Pointeur_As *Racine , string255 *Word)
    {
      /** Variables locales **/
      string255  Trouve_min_sup2 ;
      string255 Result;
      Pointeur_As Current=NULL;

      /** Corps du module **/
     Result = malloc(255 * sizeof(char));
     Trouve_min_sup2 = malloc(255 * sizeof(char));
     strcpy (Result,   "") ;
     Current  =  *Racine ;
     while( Current != NULL)  {
       if(strcmp( Info_As ( Current ), *Word) >= 0 ) {
         strcpy (Result,   Info_As ( Current )) ;
         Current  =  Fg_As ( Current ) ;
         }
       else
         {
         Current  =  Fd_As ( Current ) ;

       } ;

 } ;
     strcpy (Trouve_min_sup2,   Result) ;

     return Trouve_min_sup2 ;
    }
  string255  Trouve_max_inf (Pointeur_As *Racine , string255 *Word)
    {
      /** Variables locales **/
      string255  Trouve_max_inf2 ;
      string255 Result;
      Pointeur_As Current=NULL;

      /** Corps du module **/
     Result = malloc(255 * sizeof(char));
     Trouve_max_inf2 = malloc(255 * sizeof(char));
     strcpy (Result,   "") ;
     Current  =  *Racine ;
     while( Current != NULL)  {
       if(strcmp( Info_As ( Current ), *Word) <= 0 ) {
         strcpy (Result,   Info_As ( Current )) ;
         Current  =  Fd_As ( Current ) ;
         }
       else
         {
         Current  =  Fg_As ( Current ) ;

       } ;

 } ;
     strcpy (Trouve_max_inf2,   Result) ;

     return Trouve_max_inf2 ;
    }
  void Print_words_in_range (Pointeur_As *Racine , string255 *Word1 , string255 *Word2)
    {
      /** Variables locales **/
      string255 Lower_bound;
      string255 Upper_bound;
      Pointeur_As Current=NULL;

      /** Corps du module **/
     Lower_bound = malloc(255 * sizeof(char));
     Upper_bound = malloc(255 * sizeof(char));
     strcpy (Lower_bound,   Trouve_min_sup ( & *Racine , & *Word1 )) ;
     strcpy (Upper_bound,   Trouve_max_inf ( & *Racine , & *Word2 )) ;
     if( (strcmp( Lower_bound, "") != 0  ) && (strcmp( Upper_bound, "") != 0  ) && (strcmp( Lower_bound, Upper_bound) <= 0  )) {
       Current  =  *Racine ;
       Parcours_infixe_range ( & Current , & Lower_bound , & Upper_bound ) ;
       }
     else
       {
       printf ( " %s", "Aucun mot dans lintervalle." ) ;

     } ;

    }
  void Parcours_infixe_range (Pointeur_As *Current , string255 *Lower , string255 *Upper)
    {
      /** Variables locales **/
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     if( *Current != NULL)   {
       if(strcmp( *Lower, Info_As ( *Current )) < 0 ) {
         _Px1 =  Fg_As ( *Current ) ;
         Parcours_infixe_range ( &_Px1, & *Lower , & *Upper ) ;

       } ;
       if( (strcmp( Info_As ( *Current ), *Lower) > 0  ) && (strcmp( Info_As ( *Current ), *Upper) <= 0  )) {
         printf ( " %s", Info_As(*Current) ) ;

       } ;
       if(strcmp( Info_As ( *Current ), *Upper) <= 0 ) {
         _Px2 =  Fd_As ( *Current ) ;
         Parcours_infixe_range ( &_Px2, & *Lower , & *Upper ) ;

       } ;

     } ;

    }
  int  Compte_yza (Pointeur_As *Racine)
    {
      /** Variables locales **/
      int  Compte_yza2 ;
      Pointeur_As Current=NULL;
      int Count;
      Pointeur_PAs P=NULL;

      /** Corps du module **/
     Count  =  0 ;
     if( *Racine != NULL)   {
       Empiler_PAs (& P , *Racine ) ;
       while( ! Pilevide_PAs ( P ))  {
         Depiler_PAs (& P , &Current ) ;
         if( (strcmp( Caract ( Info_As ( Current ) , 1 ), "Y") == 0  ) || (strcmp( Caract ( Info_As ( Current ) , 1 ), "Z") == 0  ) || (strcmp( Caract ( Info_As ( Current ) , 1 ), "a") == 0  ))   {
           Count  =  Count + 1 ;

         } ;
         if( Fd_As ( Current ) != NULL)   {
           Empiler_PAs (& P , Fd_As ( Current ) ) ;

         } ;
         if( Fg_As ( Current ) != NULL)   {
           Empiler_PAs (& P , Fg_As ( Current ) ) ;

         } ;

 } ;

     } ;
     Compte_yza2  =  Count ;

     return Compte_yza2 ;
    }
  void Inordre (Pointeur_As *Racine)
    {
      /** Variables locales **/
      Pointeur_PAs Pil=NULL;
      Pointeur_As P=NULL;
      bool Possible;

      /** Corps du module **/
     printf ( " %s", "PARCOURS ''INORDRE'' DE L''ARBRE" ) ;
     Creerpile_PAs (& Pil ) ;
     P  =  *Racine ;
     Possible  =  True ;
     while( Possible)  {
       while( P != NULL)  {
         Empiler_PAs (& Pil , P ) ;
         P  =  Fg_As ( P );
 } ;
       if( ! Pilevide_PAs ( Pil ))   {
         Depiler_PAs (& Pil , &P ) ;
         printf ( " %s", Info_As(P) ) ;
         P  =  Fd_As ( P ); }
       else
         {
         Possible  =  False;
       } ;

 } ;

    }
  void Xyz_level (Pointeur_As *Root)
    {
      /** Variables locales **/
      Pointeur_FAs F1=NULL;
      Pointeur_FAs F2=NULL;
      int Cpt_niveau;
      int Cpt_mots;
      int Niveau_actuel;
      Pointeur_As P=NULL;
      Pointeur_As Marqueur=NULL;
        Typetab_V1s T_Marqueur;

      /** Corps du module **/
     int _Izw2;for (_Izw2=0; _Izw2<1; ++_Izw2)
       T_Marqueur[_Izw2]= malloc(255 * sizeof(char));
     Cpt_niveau  =  0 ;
     Creerfile_FAs (& F1 ) ;
     Creerfile_FAs (& F2 ) ;
     strcpy(T_Marqueur [ 0 ] , "" );
     Creer_arb_As (&Marqueur , T_Marqueur , 1 )  ;
     Enfiler_FAs ( F1 , *Root ) ;
     Enfiler_FAs ( F1 , Marqueur ) ;
     Niveau_actuel  =  0 ;
     Cpt_mots  =  0 ;
     while( ! Filevide_FAs ( F1 ))  {
       Defiler_FAs ( F1 , &P ) ;
       if(strcmp( Info_As ( P ), "") == 0 )   {
         printf ( " %s", "DANS LE NIVEAU " );
         printf ( " %d", Niveau_actuel );
         printf ( " %s", " IL Y A " );
         printf ( " %d", Cpt_mots );
         printf ( " %s", " MOTS COMMENCANT PAR Y, Z OU a" ) ;
         if( ! Filevide_FAs ( F1 ))   {
           Enfiler_FAs ( F1 , Marqueur ) ;
           Niveau_actuel  =  Niveau_actuel + 1 ;
           Cpt_mots  =  0 ;

         } ;
         }
       else
         {
         if( (strcmp( Caract ( Info_As ( P ) , 1 ), "Y") == 0  ) || (strcmp( Caract ( Info_As ( P ) , 1 ), "Z") == 0  ) || (strcmp( Caract ( Info_As ( P ) , 1 ), "a") == 0  ))   {
           Cpt_mots  =  Cpt_mots + 1 ;

         } ;
         if( Fg_As ( P ) != NULL)   {
           Enfiler_FAs ( F1 , Fg_As ( P ) ) ;

         } ;
         if( Fd_As ( P ) != NULL)   {
           Enfiler_FAs ( F1 , Fd_As ( P ) ) ;

         } ;

       } ;

 } ;

    }

  int main(int argc, char *argv[])
    {
     srand(time(NULL));
     S = malloc(sizeof(Typestruct1_ss));
     S->Champ1 = malloc(255 * sizeof(char));
     S->Champ2 = malloc(255 * sizeof(char));
     Mot = malloc(255 * sizeof(char));
     Word1 = malloc(255 * sizeof(char));
     Word2 = malloc(255 * sizeof(char));
     Le_mot = malloc(255 * sizeof(char));
     Fillfichier ( & F , & N ) ;
     Menu_principale() ;
     scanf ( " %d", &Choice ) ;
     while( ( Choice != 9 ))  {
       if( ( Choice == 1 ))   {
         printf ( " %s", "LES MOTS COMMENCANT PAR X,Y OU Z SONT :" );
         printf ( " %d", Compte_yza(&Bst1) ) ;
         Menu_principale() ;
         scanf ( " %d", &Choice ) ;
         }
       else
         {
         if( ( Choice == 2 ))   {
           printf ( " %s", "LE PROFONDEUR L''ARBRE BST1 EST :" );
           printf ( " %d", Profondeur(&Bst1) ) ;
           Menu_principale() ;
           scanf ( " %d", &Choice ) ;
           }
         else
           {
           if( ( Choice == 3 ))   {
             printf ( " %s", "LE PROFONDEUR L''ARBRE BST2 EST :" );
             printf ( " %d", Profondeur(&Bst2) ) ;
             Menu_principale() ;
             scanf ( " %d", &Choice ) ;
             }
           else
             {
             if( ( Choice == 4 ))   {
               printf ( " %s", "LE PROFONDEUR L''ARBRE BST3 EST :" );
               printf ( " %d", Profondeur(&Bst3) ) ;
               Menu_principale() ;
               scanf ( " %d", &Choice ) ;
               }
             else
               {
               if( ( Choice == 5 ))   {
                 printf ( " %s", "LE PARCOURS INORDRE EST :" ) ;
                 Inordre ( & Bst1 ) ;
                 Menu_principale() ;
                 scanf ( " %d", &Choice ) ;
                 }
               else
                 {
                 if( ( Choice == 6 ))   {
                   printf ( " %s", "L''ARBRE BST1:" ) ;
                   Xyz_level ( & Bst1 ) ;
                   printf ( " %s", "L''ARBRE BST2:" ) ;
                   Xyz_level ( & Bst2 ) ;
                   printf ( " %s", "L''ARBRE BST3:" ) ;
                   Xyz_level ( & Bst3 ) ;
                   Menu_principale() ;
                   scanf ( " %d", &Choice ) ;
                   }
                 else
                   {
                   if( ( Choice == 7 ))   {
                     printf ( " %s", "DONNER UN MOT A RECHERCHER" ) ;
                     scanf ( " %[^\n]", Le_mot ) ;
                     if( ( (strcmp( Caract ( Le_mot , 1 ), "Y") == 0  ) || (strcmp( Caract ( Le_mot , 1 ), "Z") == 0  ) || (strcmp( Caract ( Le_mot , 1 ), "a") == 0  ) ))   {
                       if( ( Rech_bst1 ( & Bst1 , & Le_mot ) ))   {
                         printf ( " %s", "LE MOT TROUVER" ) ;
                         }
                       else
                         {
                         printf ( " %s", "LE MOT NON TROUVER" ) ;

                       } ;
                       }
                     else
                       {
                       if( ( (strcmp( Caract ( Le_mot , 1 ), "Y") > 0  ) || (strcmp( Caract ( Le_mot , 1 ), "Z") > 0  ) || (strcmp( Caract ( Le_mot , 1 ), "a") > 0  ) ))   {
                         if( ( Rech_bst2 ( & Bst2 , & Le_mot ) ))   {
                           printf ( " %s", "LE MOT TROUVER" ) ;
                           }
                         else
                           {
                           printf ( " %s", "LE MOT NON TROUVER" ) ;

                         } ;
                         }
                       else
                         {
                         if( ( (strcmp( Caract ( Le_mot , 1 ), "Y") != 0  ) || (strcmp( Caract ( Le_mot , 1 ), "Z") != 0  ) || (strcmp( Caract ( Le_mot , 1 ), "a") != 0  ) ))   {
                           if( ( Rech_bst3 ( & Bst3 , & Le_mot ) ))   {
                             printf ( " %s", "LE MOT TROUVER" ) ;
                             }
                           else
                             {
                             printf ( " %s", "LE MOT NON TROUVER" ) ;

                           } ;

                         } ;

                       } ;

                     } ;
                     Menu_principale() ;
                     scanf ( " %d", &Choice ) ;
                     }
                   else
                     {
                     if( ( Choice == 8 ))   {
                       printf ( " %s", "DONNER LE PREMIER MOT" ) ;
                       scanf ( " %[^\n]", Word1 ) ;
                       printf ( " %s", "DONNER LE DEUXIMEE MOT" ) ;
                       scanf ( " %[^\n]", Word2 ) ;
                       if( (strcmp( Word1, Word2) < 0  ))   {
                         if( ( (strcmp( Caract ( Word1 , 1 ), "Y") == 0  ) || (strcmp( Caract ( Word1 , 1 ), "Z") == 0  ) || (strcmp( Caract ( Word1 , 1 ), "a") == 0  ) ))   {
                           Print_words_in_range ( & Bst1 , & Word1 , & Word2 ) ;
                           }
                         else
                           {
                           if( ( (strcmp( Caract ( Word1 , 1 ), "Y") > 0  ) || (strcmp( Caract ( Word1 , 1 ), "Z") > 0  ) || (strcmp( Caract ( Word1 , 1 ), "a") > 0  ) ))   {
                             Print_words_in_range ( & Bst2 , & Word1 , & Word2 ) ;
                             }
                           else
                             {
                             if( ( (strcmp( Caract ( Word1 , 1 ), "Y") != 0  ) || (strcmp( Caract ( Word1 , 1 ), "Z") != 0  ) || (strcmp( Caract ( Word1 , 1 ), "a") != 0  ) ))   {
                               Print_words_in_range ( & Bst3 , & Word1 , & Word2 ) ;

                             } ;

                           } ;

                         } ;
                         }
                       else
                         {
                         if( (strcmp( Word1, Word2) > 0  ))   {
                           if( ( (strcmp( Caract ( Word2 , 1 ), "Y") == 0  ) || (strcmp( Caract ( Word2 , 1 ), "Z") == 0  ) || (strcmp( Caract ( Word2 , 1 ), "a") == 0  ) ))   {
                             Print_words_in_range ( & Bst1 , & Word2 , & Word1 ) ;
                             }
                           else
                             {
                             if( ( (strcmp( Caract ( Word2 , 1 ), "Y") > 0  ) || (strcmp( Caract ( Word2 , 1 ), "Z") > 0  ) || (strcmp( Caract ( Word2 , 1 ), "a") > 0  ) ))   {
                               Print_words_in_range ( & Bst2 , & Word2 , & Word1 ) ;
                               }
                             else
                               {
                               if( ( (strcmp( Caract ( Word2 , 1 ), "Y") != 0  ) || (strcmp( Caract ( Word2 , 1 ), "Z") != 0  ) || (strcmp( Caract ( Word2 , 1 ), "a") != 0  ) ))   {
                                 Print_words_in_range ( & Bst3 , & Word2 , & Word1 ) ;

                               } ;

                             } ;

                           } ;

                         } ;

                       } ;
                       Menu_principale() ;
                       scanf ( " %d", &Choice ) ;
                       }
                     else
                       {
                       if( ( Choice == 9 ))   {
                         printf ( " %s", "FIN DE PROGRAMME" ) ;
                         }
                       else
                         {
                         printf ( " %s", "Enter A Valid Choice" ) ;
                         scanf ( " %d", &Choice ) ;

                       } ;

                     } ;

                   } ;

                 } ;

               } ;

             } ;

           } ;

         } ;

       } ;

 } ;


      system("PAUSE");
      return 0;
    }
