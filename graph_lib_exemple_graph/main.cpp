#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    Graph g;
    g.CHARGER_Graph_1("graph_2.txt","matrice_adj_graph_2.txt");
    for(int i(0);i<g.getordre();i++)
    {
        g.effacer_sommet(i);
    }

    g.CHARGER_Graph_1("graph_1.txt","matrice_adj_graph_1.txt");
     for(int i(0);i<g.getordre();i++)
    {
        g.effacer_sommet(i);
    }
    g.CHARGER_Graph_1("graph_2.txt","matrice_adj_graph_2.txt");




    //g.ajouter_sommet(g.getcopievertex()[0]);
    //g.ajouter_sommet(g.getcopievertex()[1]);
    //g.update_aretes();



    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


