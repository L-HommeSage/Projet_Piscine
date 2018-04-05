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





    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();
        if(key[KEY_S])
        {
            g.SAUVEGARDER_GRAPH("graph_2_nouv.txt","matrice_adj_graph_2_nouv.txt");
            std::cout<<"sauvegarde"<<std::endl;
        }

        if(key[KEY_E])
        {
            g.effacer_sommet(1);
        }

         if(key[KEY_F])
        {
            g.ajouter_sommet(g.getcopievertex()[0]);
        }


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


