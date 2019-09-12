//standard includes
#include <iostream> 
#include <fstream>
#include <cassert>

using namespace std;

//uses filtered traits

//represetnation chosen
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Representation;

#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Apollonius_graph_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Apollonius_graph_filtered_traits_2.h> 

//typedef for the traits; filtered traits class is used
typedef CGAL::Apollonius_graph_traits_2<Representation> Traits;

//typedef for the algo

/*
second template argument for vertex base class is true by default, but set explicitly here
if false, then no hidden sites will be stored. We want the hiddent sites to be stored because they could be future circles
We will be inserting, displacing, and removing sites. 
*/
typedef CGAL::Apollonius_graph_vertex_base_2<Traits,true> VertexBase;
typedef CGAL::Triangulation_face_base_2<Traits> FaceBase;
typedef CGAL::Triangulation_data_structure_2<VertexBase,FaceBase> ApolloniusGraphDataStructure;
typedef CGAL::Apollonius_graph_2<Traits,ApolloniusGraphDataStructure> ApolloniusGraph;



int main()
{
    ifstream inputFileStream("data/circleSites.txt");
    assert(inputFileStream); //seems to verify the integrity of the circlesSites.txt


    ApolloniusGraph apolloniusGraph1;
    ApolloniusGraph::Site_2 CircleCenterSite; //A "site" is the center of a circle and also a vertex in the dual of the additively weigthed voronoi diagram/ apollonius graph

    while(inputFileStream >> CircleCenterSite){ //while the file keeps having lines to go through?

    apolloniusGraph1.insert(CircleCenterSite);
    } 

    //validate the Apollonius graph
    assert(apolloniusGraph1.is_valid(true,1));
    cout << endl;




    cout <<"Number of vertices/circles: " <<apolloniusGraph1.number_of_vertices() << endl;


    //these below don't work
    //CGAL::Apollonius_graph_2<Traits,ApolloniusGraphDataStructure>::Face potato;

    //potato = apolloniusGraph1.finite_vertices_begin();
    //apolloniusGraph1.incident_vertices(apolloniusGraph1.infinite_vertex());

    //Triangulation_data_structure_2::Vertex potato = apolloniusGraph1.finite_vertex();
    
    //cout <<"Geometric Traits: " << apolloniusGraph1.geom_traits() << endl;
    //cout <<" Returns infinite vertex: " << apolloniusGraph1.incident_vertices(apolloniusGraph1.infinite_vertex()) << endl;


    /*
    
    cout <<" : " << apolloniusGraph1.  << endl;
    cout <<" : " << apolloniusGraph1.  << endl;
    cout <<" : " << apolloniusGraph1.  << endl;
    cout <<" : " << apolloniusGraph1.  << endl;
    cout <<" : " << apolloniusGraph1.  << endl;
    cout <<" : " << apolloniusGraph1.  << endl;
`   */

    return 0;

}