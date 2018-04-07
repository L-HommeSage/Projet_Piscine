#include "grman/grman.h"
#include <iostream>

#include "graph.h"
using namespace std;

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    Graph g;
    int ok(0);
    int ok2=0;
        int choix2;
        int choix;
        int choix3;

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        while (ok==0)
        {
             system("cls");
            cout<<"************BIENVENUE************"<<endl;
        cout<<endl;
        cout<<"Voulez-vous charger le graph :"<<endl;
        cout<<"  1: Cycle de l'eau"<<endl;
        cout<<"  2: Ecosysteme de la foret"<<endl;
        cout<<"  3: Ecosysteme marin"<<endl;
        cout<<"  4: Sauvegarde: Cycle de l'eau"<<endl;
        cout<<"  5: Sauvegarde: Ecosysteme de la foret"<<endl;
        cout<<"  6: Sauvegarde: Ecosysteme marin"<<endl;

        cout<<"Votre choix: ";
        cin>>choix;
        cout<<endl;
        ok++;
        }


        if (choix==1 && ok2==0)
        {
            g.CHARGER_Graph_1("graph_1.txt","matrice_adj_graph_1.txt");
            ok2++;
        }

        if (choix==2&& ok2==0)
        {
            g.CHARGER_Graph_1("graph_2.txt","matrice_adj_graph_2.txt");
            ok2++;
        }
        if (choix==3&& ok2==0)
        {
            g.CHARGER_Graph_1("graph_3.txt","matrice_adj_graph_3.txt");
            ok2++;
        }
        if (choix==4&& ok2==0)
        {
            g.CHARGER_Graph_1("graph_1_nouv.txt","matrice_adj_graph_1_nouv.txt");
            ok2++;
        }
        if (choix==5&& ok2==0)
        {
            g.CHARGER_Graph_1("graph_2_nouv.txt","matrice_adj_graph_2_nouv.txt");
            ok2++;
        }
        if (choix==6&& ok2==0)
        {
            g.CHARGER_Graph_1("graph_3_nouv.txt","matrice_adj_graph_3_nouv.txt");
            ok2++;
        }
            g.update();
            if(ok2==1)
            {
                cout<<endl;
                system("cls");
            cout<<endl;
            cout<<"( Apuyez sur E ): Ajouter un sommet"<<endl;
            cout<<"( Apuyez sur R ): Ajouter une arete"<<endl;
            cout<<"( Apuyez sur F ): Effacer un sommet"<<endl;
            cout<<"( Apuyez sur S ): Sauvegarder le graph"<<endl;
            cout<<"( Apuyez sur T ): Etudier la forte connexite"<<endl;
            cout<<"( Apuyez sur Y ): Simulation"<<endl;
            cout<<"( Apuyez sur C ): Composantes fortement connexes"<<endl;
            cout<<"( Apuyez sur I ): Retour"<<endl;


        cout<<endl;
        ok2++;
            }

            if(key[KEY_E])
            {
                g.ajouter_sommet();
                ok2=1;
            }
             if(key[KEY_R])
            {
                g.ajouter_arete();
                ok2=1;
            }
             if(key[KEY_F])
            {
                cout<<"Quel indice ?"<<endl;
                cin>>choix3;
                g.effacer_sommet(choix3);
                ok2=1;
            }
            if(key[KEY_S])
            {
                if(choix==1)
                {
                    g.SAUVEGARDER_GRAPH("graph_1_nouv.txt","matrice_adj_graph_1_nouv.txt");

                }
                if(choix==2)
                {
                    g.SAUVEGARDER_GRAPH("graph_2_nouv.txt","matrice_adj_graph_2_nouv.txt");
                }
                if(choix==3)
                {
                    g.SAUVEGARDER_GRAPH("graph_3_nouv.txt","matrice_adj_graph_3_nouv.txt");
                }
                if(choix==4)
                {
                    g.SAUVEGARDER_GRAPH("graph_1_nouv.txt","matrice_adj_graph_1_nouv.txt");
                }
                if(choix==5)
                {
                    g.SAUVEGARDER_GRAPH("graph_2_nouv.txt","matrice_adj_graph_2_nouv.txt");
                }
                if(choix==6)
                {
                    g.SAUVEGARDER_GRAPH("graph_3_nouv.txt","matrice_adj_graph_3_nouv.txt");
                }
                ok2=1;
            }

            if(key[KEY_I])
            {
                for(int i(0);i<g.getordre();i++)
                {
                    g.effacer_sommet(i);
                }

                ok=0;
                ok2=0;
            }

            if(key[KEY_Y])
            {

                g.simulation();

            }
            if(key[KEY_C])
            {
                g.toutesLesComposantesFortementConnexes();
            }

        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


