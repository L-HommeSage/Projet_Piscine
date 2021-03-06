#include "graph.h"
#include <fstream>
/***************************************************
                    VERTEX
****************************************************/
using namespace std;
/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
    pose_X = m_interface->m_top_box.get_posx();
    pose_Y = m_interface->m_top_box.get_posy();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
    if(m_weight==0)
        m_weight=0.01;
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCROSE);


    /*m_top_box.add_child(m_sauvegard);
    m_sauvegard.set_dim(20,20);
    m_sauvegard.set_pos(200,572);
    m_sauvegard.set_bg_color(ROUGE);

    m_top_box.add_child(m_legendesauvegarde);
    m_legendesauvegarde.set_message("<- Sauvegarde");
    m_legendesauvegarde.set_pos(225, 580);
    m_sauvegard.set_bg_color(ROUGE);*/


}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    m_ordre=m_vertices.size();
}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex_reduit(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices_reduit.find(idx)!=m_vertices_reduit.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices_reduit[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);
}

void Graph::add_interfaced_edge_reduit(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges_reduit.find(idx)!=m_edges_reduit.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices_reduit.find(id_vert1)==m_vertices_reduit.end() || m_vertices_reduit.find(id_vert2)==m_vertices_reduit.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices_reduit[id_vert1], m_vertices_reduit[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges_reduit[idx] = Edge(weight, ei);
    m_edges_reduit[idx].m_from = id_vert1;
    m_edges_reduit[idx].m_to = id_vert2;
    m_vertices_reduit[id_vert1].m_out.push_back(id_vert2);
    m_vertices_reduit[id_vert2].m_in.push_back(id_vert1);
}

void Graph::effacer_sommet(int eidx)
{

    for(auto &e: m_edges)
    {
        if(e.second.m_from==eidx || e.second.m_to==eidx)
        {
            effacer_arete(e.first);
        }
    }
    Vertex &remver=m_vertices.at(eidx);
    if(m_interface && remver.m_interface)
    {
        m_interface->m_main_box.remove_child( remver.m_interface->m_top_box );
    }

    m_ordre--;
    m_vertices.erase( eidx );
}

void Graph::effacer_arete(int eidx)
{

    Edge &remed=m_edges.at(eidx);
    if (m_interface && remed.m_interface)
    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );
    m_edges.erase( eidx );
}

void Graph::ajouter_sommet()
{
    int indice=0;
    bool indice_est_utilise;
    double valeur;
    string image;
    for(int i = 0; i <= m_vertices.size(); i++)
    {
        indice_est_utilise = false;
        for(auto &e : m_vertices)
        {
            if(e.first == i)
            {
                indice_est_utilise=true;
            }
        }
        if(indice_est_utilise==false)
        {
            indice=i;
            break;
        }
    }
    cout<<"saisir valeur: ";
    cin>>valeur;
    std::cout<<endl;
    std::cout<<"saisir image: ";
    cin>>image;
    std::cout<<endl;
    image=image+".jpg";

    m_ordre++;
    add_interfaced_vertex(indice, valeur, 250, 250,image);

}

void Graph::ajouter_arete()
{
    int from,to;
    double poids;
    std::cout<<"saisir sommet de depart: ";
    std::cin>>from;
    std::cout<<endl;
    cout<<"saisir sommet d arrive: ";
    cin>>to;
    std::cout<<endl;
    cout<<"saisir poids de l arete: ";
    cin>>poids;
    std::cout<<endl;

    add_interfaced_edge(m_edges.size()+1,from,to,poids);
}
void Graph::CHARGER_Graph_1(string fic1, string fic2)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ifstream fichier(fic1, ios::in);
    if(fichier)
    {
        int indice;
        double valeur;
        int posex,posey;
        string image;
        int ordre;
        fichier>>ordre;
        m_ordre=ordre;

        for(int i(0); i<ordre; ++i)
        {
            fichier>>indice;
            fichier>>valeur;
            fichier>>posex;
            fichier>>posey;
            fichier>>image;
            add_interfaced_vertex(indice, valeur, posex, posey, image);
            m_vertices[indice].m_image=image;
        }
        fichier.close();
    }
    else
        cout << "Impossible d'ouvrir le fichier !" << endl;


    ifstream fichier2(fic2, ios::in);
    if(fichier2)
    {
        double poids;
        int indice_arc=0;
        for(int j(0); j<m_ordre ; ++j)
        {
            for(int k(0); k<m_ordre; k++)
            {
                fichier2>>poids;
                if(poids!=0)
                {
                    add_interfaced_edge(indice_arc, j, k, poids);


                    indice_arc++;


                }
            }
        }
        fichier2.close();
    }
    else
        cout << "Impossible d'ouvrir le fichier !" << endl;

}

void Graph::SAUVEGARDER_GRAPH (string fic1, string fic2)
{
    ofstream fichier1(fic1, ios::trunc);
    if(fichier1)
    {
        fichier1 << m_ordre << endl;

        for (auto &e : m_vertices)
        {
            fichier1 << e.first << ' ' << e.second.m_value << ' ' << e.second.pose_X << ' ' ;
            fichier1 << e.second.pose_Y << ' ' << e.second.m_image << endl;
        }
        fichier1.close();
    }
    else
        cout << "Impossible d'ouvrir le fichier !" << endl;

    ///Sauvegarde des arêtes

    ofstream fichier2(fic2, ios::trunc);

    if(fichier2)
    {
        for(int i = 0; i < m_ordre; i++)
        {
            for(int j = 0; j < m_ordre; j++)
            {
                int trouve = 0;
                for (auto &e : m_edges)
                {
                    if (e.second.m_from==i && e.second.m_to==j )
                    {
                        trouve= e.second.m_weight;
                    }

                }
                fichier2 << trouve << ' ';
            }
            fichier2<<endl;
        }

        fichier2.close();
    }
    else
        cout << "Impossible d'ouvrir le fichier !" << endl;
}

void Graph::Marche_ecosysteme()
{
    std::map<int,double> valeurs;
    double valeur;

    for(auto &vert : m_vertices)
    {
        double k=0;
        double predation=0;
        bool pred=false;
        for (auto &ed : m_edges)
        {
            if( ed.second.m_to==vert.first)
            {
                k+=ed.second.m_weight*0.01*m_vertices[ed.second.m_from].m_value;
                pred=true;
            }
            if(ed.second.m_from==vert.first)
            {
                predation+=ed.second.m_weight*0.01*m_vertices[ed.second.m_to].m_value;
            }
        }
        if(k==0 && pred==false)
        {
            k=100;

        }

        if(k==0 && pred==true)
        {
            k=0.1;
        }


        //cout<<vert.second.m_value<<endl;

        valeur = vert.second.m_value+0.05*(vert.second.m_value*(1-vert.second.m_value/k)-predation);
        //cout<<predation<<endl;
        valeurs.insert(std::make_pair(vert.first,valeur));
    }
    for( auto &valor: valeurs)
    {
        for (auto &vertices : m_vertices)
        {
            if(vertices.first == valor.first)
            {
                vertices.second.m_value=valor.second;
            }
        }
    }



rest(25);

}

vector<bool> Graph::uneComposanteFortementConnexe(int s, vector<vector<bool>> tab_adjacence)
{
    vector<bool> c1(m_ordre,0); //composantes connexes directes partant de s
    vector<bool> c2(m_ordre,0); //et indirectes arrivant vers s
    vector<bool> c(m_ordre,0); //composante fortement connexe = c1 & c2 à retourner
    vector<bool> marques(m_ordre,0); //tableau dynamique indiquant si les sommets sont marqués ou non
    int x,y; //numéros de sommets intermédiaires des composantes connexes
    bool ajoute = 1 ; // booléen indiquant si une nouvelle composante connexe est ajoutée

    //Le sommet s est rendu connexe
    c1[s] = 1;
    c2[s]=1;

    //Recherche des composantes connexes partant de s à ajouter dans c1 :
    while(ajoute)
    {
        ajoute = 0;

        //Pour tous les sommets x non marqués et connectés en partant de s
        // Marquer chaque sommet x et connecter les sommets non marqués y adjacents à x
        for(x=0; x<m_ordre; x++)
        {
            if(!marques[x] && c1[x])
            {
                marques[x]=1;
                for(y=0; y<m_ordre; y++)
                {
                    if(tab_adjacence[x][y] && !marques[y])
                    {
                        c1[y]=1;
                        ajoute = 1;
                    }
                }
            }
        }
    }

    //Recherche des composantes connexes arrivant à s à ajouter dans c2 :
    for(int i = 0; i<marques.size(); i++)
    {
        marques[i]=0;
    }
    ajoute = 1;
    while(ajoute)
    {
        ajoute = 0;

        //Pour tous les sommets x non marqués et connectés arrivant à s
        // Marquer chaque sommet x et connecter les sommets non marqués y adjacents à x
        for(x=0; x<m_ordre; x++)
        {
            if(!marques[x] && c2[x])
            {
                marques[x]=1;
                for(y=0; y<m_ordre; y++)
                {
                    if(tab_adjacence[y][x] && !marques[y])
                    {
                        c2[y]=1;
                        ajoute = 1;
                    }
                }
            }
        }
    }

    // Composante fortement connexe c = intersection de c1 et c2
    for (x=0 ; x<m_ordre ; x++)
    {
        c[x] = c1[x] & c2[x] ;
    }

    // Retourner la composante fortement connexe c
    return c ;
}

void Graph::toutesLesComposantesFortementConnexes()
{
    ComposantesFortementConnexes.clear();
    /// ////////Création d'une matrice d'adjacence /////////////
    vector<vector<bool>> tab_adjacence;
    for(int i=0; i<m_ordre; i++)
    {
        vector<bool> temp(m_ordre,0);
        tab_adjacence.push_back(temp);
    }
    for(auto &arc: m_edges)
    {
        int x = arc.second.m_from;
        int y = arc.second.m_to;
        tab_adjacence[x][y]=1;
    }
    /// ///////////////////////////////////////////////////////

    vector<bool> marques(m_ordre,0); //tableau dynamique indiquant si les sommets sont marqués ou non
    int x, y ; //numéros de sommets intermédiaires comme indices des tableaux
    int nb_composantes=0;

    // Pour tous les sommets x non marqués
    // Rechercher la composante fortement connexe de x
    // Marquer chaque sommet x et marquer les sommets y connectés à x et non marqués
    for (x=0 ; x<m_ordre ; x++)
    {
        if (!marques[x])
        {
            /*vector<bool> composante = uneComposanteFortementConnexe(x, tab_adjacence);
            for(int k =0; k < composante.size(); k++)
                cout << composante[k] << " ";
            cout << endl;*/
            ComposantesFortementConnexes.push_back(uneComposanteFortementConnexe(x, tab_adjacence)) ;

            marques[x] = 1 ;

            //On marque tous les sommets connexes à x
            for(y = 0; y<ComposantesFortementConnexes[nb_composantes].size(); y++)
            {
                if(ComposantesFortementConnexes[nb_composantes][y] && !marques[y])
                {
                    marques[y] = 1;
                }
            }

            nb_composantes++;
            cout << "Composante " << nb_composantes << ": (" ;
            for(int i=0; i<ComposantesFortementConnexes[nb_composantes-1].size(); i++)
            {
                if(ComposantesFortementConnexes[nb_composantes-1][i])
                {
                    cout << i << " ";
                }
            }
            cout << " )" << endl;
        }
    }
    cout << "Il y a " << ComposantesFortementConnexes.size() << " composante fortement connexes." << endl;
}

void Graph::cacher_graph()
{
    for(auto &elem : m_vertices)
    {
        if(m_interface && elem.second.m_interface)
        {
            m_interface->m_main_box.remove_child( elem.second.m_interface->m_top_box );
        }
    }

    for(auto &elem : m_edges)
    {
        if (m_interface && elem.second.m_interface)
        {
            m_interface->m_main_box.remove_child( elem.second.m_interface->m_top_edge );
        }
    }
}

void Graph::afficher_graph()
{
    m_vertices_reduit.clear();
    m_edges_reduit.clear();

    for(auto &elem : m_vertices)
    {
        if(m_interface && elem.second.m_interface)
        {
            m_interface->m_main_box.add_child( elem.second.m_interface->m_top_box );
        }
    }

    for(auto &elem : m_edges)
    {
        if (m_interface && elem.second.m_interface)
        {
            m_interface->m_main_box.add_child( elem.second.m_interface->m_top_edge );
        }
    }
}

void Graph::marquerSommets()
{
    vector<long> couleurs= {VIOLETCLAIR,JAUNE,VERT,GRISCLAIR,ROSE,ROSESOMBRE,ROUGE,ROUGECLAIR,ROUGESOMBRE,VERTCLAIR,VERTFLUO,BLEU,BLEUSOMBRE,MARRON,MARRONCLAIR,MARRONSOMBRE};
    for(int i=0; i<ComposantesFortementConnexes.size(); i++)
    {
        for(int j=0; j< ComposantesFortementConnexes[i].size(); j++)
        {
            if(ComposantesFortementConnexes[i][j])
            {
                m_vertices[j].m_interface->m_top_box.set_bg_color(couleurs[i]);
            }
        }
    }
}

void Graph::demarquerSommets()
{
    for(auto &elem : m_vertices)
        elem.second.m_interface->m_top_box.set_bg_color(BLANC);
}

void Graph::creer_graph_reduit()
{
    for(int i = 0; i<ComposantesFortementConnexes.size(); i++)
    {
        int j=0;
        while(!ComposantesFortementConnexes[i][j])
        {
            j++;
        }
        add_interfaced_vertex_reduit(i, 50, m_vertices[j].pose_X, m_vertices[j].pose_Y /*,m_vertices[j].m_image*/);
    }

    for(auto &elem : m_edges)
    {
        int x=-1;
        int y=-1;
        int from = elem.second.m_from;
        int to = elem.second.m_to;
        for(int i = 0; i<ComposantesFortementConnexes.size(); i++)
        {
            if(ComposantesFortementConnexes[i][from])
            {
                x=i;
            }
            if(ComposantesFortementConnexes[i][to])
            {
                y=i;
            }
        }
        if(x!=-1 && y!=-1 && x!=y)
        {
            add_interfaced_edge_reduit(m_edges_reduit.size()+1,x,y,elem.second.m_weight);
        }
    }
}

void Graph::cacher_graph_reduit()
{
    for(auto &elem : m_vertices_reduit)
    {
        if(m_interface && elem.second.m_interface)
        {
            m_interface->m_main_box.remove_child( elem.second.m_interface->m_top_box );
        }
    }

    for(auto &elem : m_edges_reduit)
    {
        if (m_interface && elem.second.m_interface)
        {
            m_interface->m_main_box.remove_child( elem.second.m_interface->m_top_edge );
        }
    }
}


