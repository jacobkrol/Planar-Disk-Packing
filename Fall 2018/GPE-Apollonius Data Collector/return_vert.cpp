typedef CGAL::Apollonius_graph_traits_2<Kernel_Exact>      		           APT;
typedef CGAL::Apollonius_site_2<Kernel_Exact>               		           Site_2_Apo;
typedef Site_2_Apo::Point_2              		                           Site_2_Point_2;
typedef Site_2_Apo::Weight                                   			   Site_2_Weight;

typedef CGAL::Apollonius_graph_traits_2<Kernel_Exact>                              AGT2_K;
typedef CGAL::Apollonius_graph_2<AGT2_K>                                           AG2;
typedef CGAL::Apollonius_graph_adaptation_traits_2<AG2>                            AG2_Trait;
typedef CGAL::Apollonius_graph_caching_degeneracy_removal_policy_2<AG2>            AG2_Policy;
typedef CGAL::Voronoi_diagram_2<AG2,AG2_Trait,AG2_Policy>                          VD_AG2;


main(){
	return 0;
}
