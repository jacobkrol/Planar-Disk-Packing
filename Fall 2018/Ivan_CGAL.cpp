#include <fstream>
#include <string>
#include <cassert>
#include <ctime>
#include <bits/stdc++.h>
#define TOLERABLYCLOSE 0.01




// CGAL headers
//
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>


#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Apollonius_graph_hierarchy_2.h>
#include <CGAL/Apollonius_graph_filtered_traits_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Origin.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Aff_transformation_2.h>





#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>

// Qt headers
#include <QtGui>
#include <QString>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QMouseEvent>

// GraphicsView items and event filters (input classes)
#include <CGAL/Qt/ApolloniusGraphGraphicsItem.h>
#include <CGAL/Qt/GraphicsViewCircleInput.h>

// for viewportsBbox
#include <CGAL/Qt/utility.h>
  

// the two base classes
#include "ui_Apollonius_graph_2.h"
#include <CGAL/Qt/DemosMainWindow.h>




typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_2 Point_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;
typedef K::Vector_2                       Vector_2;
//typedef K::Point_2                        Point_2;
typedef K::Direction_2                      Direction_2;
typedef K::Aff_transformation_2             Transformation;



//CGAL::Integral_domain_without_division_tag
typedef CGAL::Apollonius_graph_filtered_traits_2<K>  Gt;
//typedef CGAL::Triangulation_2<K,CGAL::Integral_domain_without_division_tag> TriTraits;

typedef Gt::Point_2                           Point_2;
typedef K::Circle_2                         Circle_2;
typedef Gt::Site_2                            Apollonius_site_2;
typedef Gt::Site_2::Weight                    Weight;
typedef Gt::Object_2                        Object_2;
typedef Gt::Line_2                          Line_2;
//typedef GT::Vector_2                       Vector_2;





//typedef CGAL::Apollonius_graph_2<Gt> Apollonius;
typedef CGAL::Apollonius_graph_hierarchy_2<Gt> Apollonius;
typedef CGAL::Triangulation_2<Gt> Triangulation;
typedef Apollonius::Vertex_handle Vertex_handle;
typedef Apollonius::Face_handle Face_handle;
typedef Apollonius::Site_2 Site_2;
typedef Apollonius::Vertex_circulator Vertex_circulator;


constexpr qreal operator "" _qr(long double a){ return qreal(a); }

//template function for determining intersection or tangent, returns 1 if outside circle, -1 if inside circle, 0 if tangent to circle
template <typename T>
short sign(T val) {
    return (T(0) < val) - (val < T(0));
}

short IsIntersecting(Apollonius::Site_2 &siteA, Apollonius::Site_2 &siteB){

return sign <K::FT> (CGAL::sqrt(CGAL::square(siteA.point().x() - siteB.point().x() )+CGAL::square(siteA.point().y() - siteB.point().y())) - siteA.weight() - siteB.weight());
}

double Distance(Apollonius::Site_2 &siteA, Apollonius::Site_2 &siteB){

    return //(CGAL::sqrt(CGAL::square(siteA.point().x() - siteB.point().x() )+CGAL::square(siteA.point().y() - siteB.point().y())) - siteA.weight() - siteB.weight());
            (CGAL::sqrt(CGAL::squared_distance(siteA.point(),siteB.point()))-siteA.weight()-siteB.weight());
}

double PALLETESIZE = 0;


class MainWindow :
  public CGAL::Qt::DemosMainWindow,
  public Ui::Apollonius_graph_2
{
  Q_OBJECT

private:
  Apollonius ag;
  QGraphicsScene scene;

  CGAL::Qt::ApolloniusGraphGraphicsItem<Apollonius,K> * agi;
  CGAL::Qt::GraphicsViewCircleInput<K> * ci;

public:
  MainWindow();

public Q_SLOTS:

  void processInput(CGAL::Object o);

  void on_actionInsertRandomPoints_triggered();

  void on_actionLoadPoints_triggered();

  void on_actionSavePoints_triggered();

  void on_actionClear_triggered();

  void on_actionRecenter_triggered();

  //void onStopClicked();

  virtual void open(QString fileName);

Q_SIGNALS:
  void changed();
};


MainWindow::MainWindow() :
    DemosMainWindow()
{
  setupUi(this);

  this->graphicsView->setAcceptDrops(false);

  agi = new CGAL::Qt::ApolloniusGraphGraphicsItem<Apollonius, K>(&ag);

  QObject::connect(this, SIGNAL(changed()),
           agi, SLOT(modelChanged()));

  agi->setSitesPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  agi->setEdgesPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  //agi->setEdgesPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  scene.addItem(agi);
  //agi->

  // Setup input handlers. They get events before the scene gets them
  // and the input they generate is passed to the triangulation with
  // the signal/slot mechanism

  ci = new CGAL::Qt::GraphicsViewCircleInput<K>(this, &scene);
  /*
  QObject::connect(ci, SIGNAL(generate(CGAL::Object)),
           this, SLOT(processInput(CGAL::Object)));

  //this->onStopClicked();
*/
  //scene.installEventFilter(ci);

  //
  // Manual handling of actions
  //

  //QObject::connect(this->actionQuit, SIGNAL(triggered()),
  //         this, SLOT(close()));

  //
  // Setup the scene and the view
  //

  scene.setItemIndexMethod(QGraphicsScene::NoIndex);
  scene.setSceneRect(0, 0, 100, 100);
  this->graphicsView->setScene(&scene);
  this->graphicsView->setMouseTracking(true);
  //this->grapphicsView->mouseMoveEvent();
  this->graphicsView->scale(1,-1);
  std::cout << "SCREEN RESET/FLIPPED" << std::endl;


  // Turn the vertical axis upside down
    //this->graphicsView->frameRect(); //matrix().scale(.5, -.5);
  //this->graphicsView->centerOn(100,100);
  //this->graphicsView->fitInView(agi);

  //double nom = (PALLETESIZE / (double)100.0) * (double)5.0;
  //std::cout<< nom << std::endl;

          //(((double)(PALLETESIZE))/100)*5.0,(((double)(PALLETESIZE))/100)*5.0);


  // The navigation adds zooming and translation functionality to the
  // QGraphicsView
  this->addNavigation(this->graphicsView);

  this->setupStatusBar();
  this->setupOptionsMenu();
  this->addAboutDemo(":/cgal/help/about_Apollonius_graph_2.html");
  this->addAboutCGAL();

  this->addRecentFiles(this->menuFile, this->actionQuit);
  connect(this, SIGNAL(openRecentFile(QString)),
	  this, SLOT(open(QString)));

  //qreal num = 7;
  //this->graphicsView->scale(num,num);
}



void
MainWindow::processInput(CGAL::Object o)
{
  std::pair<Point_2, double> center_and_sr;
  if(CGAL::assign(center_and_sr, o)){
    ag.insert(Apollonius_site_2(center_and_sr.first, sqrt(center_and_sr.second)));
    //std::cout <<
    Q_EMIT( changed());
  }
}


/*
 *  Qt Automatic Connections
 *  http://doc.qt.io/qt-5/designer-using-a-ui-file.html#automatic-connections
 *
 *  setupUi(this) generates connections to the slots named
 *  "on_<action_name>_<signal_name>"
 */


void
MainWindow::on_actionClear_triggered()
{
  ag.clear();
  Q_EMIT( changed());
}

double Radius(double &radius1, double &radius2, short &radiusAlt,int &ratio){
    if (radiusAlt % ratio == 0){
        return radius1;
    }else{
        return radius2;
    }

 }


void
MainWindow::on_actionInsertRandomPoints_triggered()
{
    scene.setSceneRect(0, 0, PALLETESIZE, PALLETESIZE);
    this->graphicsView->setScene(&scene);
    this->graphicsView->resetTransform();
    this->graphicsView->scale(1,-1);

    PALLETESIZE = QInputDialog::getDouble(this,
                                           tr("Pallete Size"),
                                           tr("Enter pallete size: "),
                                           100,
                                           0,
                                          (std::numeric_limits<double>::max)(),
                                          1);

  //QRectF rect = CGAL::Qt::viewportsBbox(&scene);
  QRectF rect(0,0,PALLETESIZE,PALLETESIZE);
  CGAL::Qt::Converter<K> convert;
  Iso_rectangle_2 isor = convert(rect);
  double width = isor.xmax() - isor.xmin();
  double n = CGAL::square(PALLETESIZE);
  qreal num =  1000.0/(2.0*pow(10.0,log10(PALLETESIZE)));
//.
  std::cout << num << std::endl;
  scene.setSceneRect(0, 0, PALLETESIZE, PALLETESIZE);
  this->graphicsView->setScene(&scene);
  this->graphicsView->scale(num,num);


  
  CGAL::Random_points_in_iso_rectangle_2<Point_2> pg((isor.min)(), (isor.max)());
  bool ok = false;

  int input =
    QInputDialog::getInt(this,
                             tr("Number of random points"),
                             tr("Enter number of random non intersecting points"),
                             -1,
                             -1,
							(std::numeric_limits<int>::max)(),
							1,
							&ok);

  if(!ok) {
    return;
  }

  // if -1 is inputted, the pallete will be saturated
  if (input == -1){
      input = INT_MAX;
  }
  const int number_of_points = input;

  // wait cursor
  //QApplication::setOverrideCursor(Qt::WaitCursor);
  //std::vector<Apollonius_site_2> points;
  //points.reserve(number_of_points);

  double insertedRadius = 0.0;
  short radiusAlternator = 0; 
  int ratioOfFirstToSecondSpecies = QInputDialog::getInt(this,
                                                            tr("n : 1 ratio for second radius species to first radius species"),
                                                            tr("n : 1  ratio for second radius species to first radius species"),
                                                            2,
                                                            0,
                                                           (std::numeric_limits<int>::max)(),
                                                           1,
                                                           &ok);

                                 if(!ok) {
                                   return;
                                 }
  double radius_1 = QInputDialog::getDouble(this,
                                            tr("Enter Radius of first Species"),
                                            tr("Enter Radius of first Species"),
                                            1.0,
                                            0,
                                           (std::numeric_limits<double>::max)(),
                                           1,
                                           &ok);

                 if(!ok) {
                   return;
                 }
  boost::rand48 rng(time(0));
  boost::uniform_real<> dist(0, 1); //radius between 0 and 1
  boost::variate_generator<boost::rand48&, boost::uniform_real<> > radius2(rng,dist); //radius of second species
  double radius_2 = QInputDialog::getDouble(this,
                                         tr("Enter Radius of second Species"),
                                         tr("Enter Radius of second Species"),
                                         .8,
                                         0,
                                        (std::numeric_limits<double>::max)(),
                                        1,
                                        &ok);

              if(!ok) {
                return;
              }
          //radius2();

  /*
  int maxNumberOfAnySpecies = 1000;
  boost::random::uniform_int_distribution<> range1(1,maxNumberOfAnySpecies);
  boost::mt19937 ratioRadius1rng(time(0));
  boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<> > NumberOfRadius1(ratioRadius1rng,range1);
  NumberOfRadius1.distribution().reset();

  boost::random::uniform_int_distribution<> range2(1,maxNumberOfAnySpecies);
  boost::mt19937 ratioRadius2rng(time(0)+1);
  boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<> > NumberOfRadius2(ratioRadius2rng,range2);
  NumberOfRadius2.distribution().reset();







  std::cout << NumberOfRadius1() << " -- " << NumberOfRadius2() << " -- " <<radius2() << " -- " <<radius1<<  std::endl;
*/


  insertedRadius = Radius(radius_1, radius_2, radiusAlternator,ratioOfFirstToSecondSpecies);
  Apollonius_site_2 firstSite(*pg++,insertedRadius);
  Vertex_handle vNearestNeighbor;
  int spotFound = 0;
  int circlesCount = 0;
  int fail0 = 0;

  //inserion of first point
  if (ag.number_of_visible_sites() > 0){ //if there is already a site inserted
      while (spotFound != 1){
          vNearestNeighbor = ag.nearest_neighbor(firstSite.point());
          if(IsIntersecting(vNearestNeighbor->site(), firstSite) != -1){
              ag.insert(firstSite);
              radiusAlternator++;
              spotFound = 1;
              circlesCount++;
              std::cout << "SPOT FOUND: " << firstSite << std::endl;


          }else{
              Apollonius_site_2 firstSite(*pg++,Radius(radius_1, radius_2, radiusAlternator,ratioOfFirstToSecondSpecies));
              std::cout << "TRY AGAIN, didn't work: " << firstSite << std::endl;
              fail0++;

          }

          if(fail0 > 10){
              break;
          }
      }
  } else if (number_of_points != 0){
      ag.insert(firstSite);
      circlesCount++;
      std::cout << "BARE INSERT: " << firstSite << std::endl;
    }


  //inserion of rest of points

  short times = 0;
  //toStop = false;
  int fail = 0;

  //begin random inserter
  //for(int i = 0; i < number_of_points; ++i)
  while(circlesCount < number_of_points){
      if(circlesCount < number_of_points){

          std::cout <<radius2() << "--" << *pg << " " << *pg << std::endl;

          Apollonius_site_2 site(*pg++,Radius(radius_1, radius_2, radiusAlternator,ratioOfFirstToSecondSpecies));
          vNearestNeighbor = ag.nearest_neighbor(site.point());
          if(IsIntersecting(vNearestNeighbor->site(), site) != -1){
              ag.insert(site);
              radiusAlternator++;
              circlesCount++;
          }else
              fail++;
          }
          if(fail > 1){
              QCoreApplication::processEvents();
              Q_EMIT( changed());
              break;
          }

          if (circlesCount % 50 == 1){
              QCoreApplication::processEvents();
              Q_EMIT( changed());
                  //std::cout << "" << std::endl;
          }

  }//end random inserter

std::cout << "This many circles put randomly: " << circlesCount << std::endl;
std::cout << "This many circles: " << number_of_points << std::endl;
std::cout <<"Number of vertices/circles: " <<ag.number_of_faces() << std::endl;





    bool ran = false;
    int VorCircs = 0;
    int faceNumber = 0;
    int fail1 = 0;
 //begin voronoi dual verticies inserter if there are still remaining circles;

  std::clock_t start;
  double duration;

 if(circlesCount < number_of_points){

    while(fail1 < 3){
     VorCircs = 0;
     for(Apollonius::Finite_faces_iterator fit = ag.finite_faces_begin(), //begin for loop for dual inserter
        end = ag.finite_faces_end(); fit!= end; ++fit){
        //std::cout << "FaceNumber: "<< ++faceNumber << std::endl;
        Apollonius_site_2 site0 = ag.dual(fit);
        if (isor.has_on_bounded_side(site0.point()) == 1){ //if the vertex is in the pallete continue
            //disk of largest radius to fit in empty space
            Apollonius_site_2 site1(site0.point(),Radius(radius_1, radius_2, radiusAlternator,ratioOfFirstToSecondSpecies));//site with our set radius to be tested

            vNearestNeighbor = ag.nearest_neighbor(site1.point());

            if(IsIntersecting(site1,vNearestNeighbor->site()) != -1){ //test for intersection with nearest neighbor
                ag.insert(site1);
                end =ag.finite_faces_end();
                QCoreApplication::processEvents();
                Q_EMIT( changed());
                radiusAlternator++;
                circlesCount++;
                VorCircs++;

          }else {
                radiusAlternator++;
                Apollonius_site_2 site2(site0.point(),Radius(radius_1, radius_2, radiusAlternator,ratioOfFirstToSecondSpecies));//site with our set radius to be tested

                vNearestNeighbor = ag.nearest_neighbor(site2.point());

                if(IsIntersecting(site2,vNearestNeighbor->site()) != -1){ //test for intersection with nearest neighbor
                    ag.insert(site2);
                    end =ag.finite_faces_end();
                    QCoreApplication::processEvents();
                    Q_EMIT( changed());
                    radiusAlternator++;
                    circlesCount++;
                    VorCircs++;
                }


            }
          //std::cout <<radius2() << "-- " << radius_2<< " Number of Vor Circles: " << VorCircs << " times: " <<std::endl;
          //std::cout <<"Number of faces: " <<ag.number_of_faces() << std::endl;
          //if (VorCircs % 50 == 1){
            //  QCoreApplication::processEvents();
              //Q_EMIT( changed());
                  //std::cout << "" << std::endl;
          }
          //std::cout << NumberOfRadius1() << " -- " << NumberOfRadius2() << " -- " <<radius2() << " -- " <<radius1<<  std::endl;
        if(circlesCount == number_of_points){
            //QCoreApplication::processEvents();
            //Q_EMIT( changed());
            break;
        }


        Apollonius::Finite_faces_iterator fit0;
        fit0 = fit;
        fit0++;
        if(++fit0 == end && ran == false){
            fit = ag.finite_faces_begin();
            end = ag.finite_faces_end();
            //fit;
            std::cout << "RESET RAN" << std::endl;

            ran = true;
        }

}// end of vor dual inserter (faces iterator) if statement, begin attractor
     if(VorCircs == 0){
         fail1++;
     }

}//end while loop
}

 duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

     std::cout<<"printf: "<< duration <<'\n';
/*
 //begin attractor
 int vertexCount = 0;
 double min = DBL_MAX;
 double weightedDistance;
 double WeightedTranslationDistance = 0; //this initialization to zero might create some problems
 bool NearestDiskIsIntersecting = false;
 bool ranVertexIteratorReset = false;

 for(Apollonius::Finite_vertices_iterator
       vit = ag.finite_vertices_begin(),
       end = ag.finite_vertices_end();
     vit!= end; ++vit)
 {
     Vertex_handle v = vit;
     //Vertex_handle closestVertex;
     Vertex_handle closestVertex;
     Vertex_circulator circulator = ag.incident_vertices(v), done(circulator); //finds the nearest vertex without removing central site
     do{
         weightedDistance = Distance(v->site(),circulator->site());
         if(weightedDistance < min && IsIntersecting(v->site(),circulator->site()) != -1){
             min = weightedDistance;
             std::cout << "NO INTERSECTION FOUND: Circulator CONTINUED to next vertex."<< std::endl;

             Vertex_handle closestNonIntersectingvVertex = circulator;
         }else if(weightedDistance < 0){
             NearestDiskIsIntersecting = true;
             Vertex_handle closestIntersectingVertex = circulator;
             std::cout << "INTERSECTION FOUND: Circulator stopped at intersecting vertex."<< std::endl;
             break;
         }
     }while(++circulator != done);//end circulator
        v = vit;
        ag.remove(closestVertex);
        vertexCount++;



        Vector_2 ClosestDiskToVitVector(closestVertex->site().point(),v->site().point()); //center to center length
        if(NearestDiskIsIntersecting == false){
            WeightedTranslationDistance = (CGAL::sqrt(CGAL::squared_distance(closestVertex->site().point(),v->site().point()))-closestVertex->site().weight()-v->site().weight());
            Transformation scaleToWeighteDistance(CGAL::SCALING,WeightedTranslationDistance);
            //Transformation MatrixTransformation((CGAL::sqrt(2)/2), 0, 0, 0, CGAL::sqrt(2)/2, 0,1);
            //ClosestDiskToVitVector = ClosestDiskToVitVector
              std::cout << "ClosestVector before transform "<< ClosestDiskToVitVector << std::endl;
            Direction_2 TowardsCenterDisk = ClosestDiskToVitVector.direction();
            double length = CGAL::sqrt(ClosestDiskToVitVector.squared_length());
            ClosestDiskToVitVector = ClosestDiskToVitVector/length; //normalized vector
             std::cout << " ClosestVector after norm "<< ClosestDiskToVitVector << std::endl;
            double UnitLength = CGAL::sqrt(CGAL::square(ClosestDiskToVitVector.x())+CGAL::square(ClosestDiskToVitVector.y()));
                    std::cout << " Unit length "<< UnitLength << std::endl;
            Apollonius_site_2 AttractedToCenterSite(closestVertex->site().point() + ClosestDiskToVitVector * WeightedTranslationDistance);
            std::cout << " WE HEA" << std::endl;
            //++vit = ag.finite_vertices_begin();
            std::cout <<"Number of vertices: " <<ag.number_of_vertices() << std::endl;


            //++vit = ag.finite_vertices_begin();
           // vit = ag.finite_vertices_begin();

            //QCoreApplication::processEvents();
            //Q_EMIT( changed());
            std::cout << " NAAH WE HEA" << std::endl;
            std::cout <<"Number of vertices: " <<ag.number_of_vertices() << std::endl;

            ag.insert(AttractedToCenterSite);
            end = ag.finite_vertices_end();
            v = vit;

            //end = ag.finite_vertices_end();
            std::cout << " HOWABOUT WE HEA" << std::endl;
            std::cout <<"Number of vertices: " <<ag.number_of_vertices() << std::endl;



        }
        Apollonius::Finite_vertices_iterator vit3;
        vit3 = vit;
        vit3++;
        if(++vit3 == end && ranVertexIteratorReset == false){
            vit = ag.finite_vertices_begin();
            end = ag.finite_vertices_end();
            //fit;
            std::cout << "ATTRACTOR RESET RAN" << std::endl;

            ranVertexIteratorReset = true;
        }

 }//end of iterator
 std::cout << "Circulated Vertices: " <<  vertexCount << std::endl;




 //End Attractor

 */

/*
    //begin attractor
    //Vertex_handle nearest;
    //Point_2 p;
    double distance = 0;
    Apollonius_site_2 site;
    Vertex_handle nearest;
    int NotReinserted =0;
    int Moved = 0;
    int PairBonded = 0;
    double LargestIntersection = 0;
    bool ranVertexIteratorReset = false;
    int intersectionCount = 0;
 while(PairBonded <ag.number_of_visible_sites()-1){
        PairBonded = 0;

    for(Apollonius::Finite_vertices_iterator
          vit = ag.finite_vertices_begin(),
          end = ag.finite_vertices_end();
        vit!= end; ++vit)
    {

        Vertex_handle v = vit;
        //double min =  9999;
        double weightedDistance = 0.0;
        Vertex_handle ClosestVertex;
        Vertex_handle IntersectingVertex;
        // ag.nearest_neighbor(v->site().point(),v);
        bool Intersecting = false;
        bool TolerablyBonded = true;
        Vertex_circulator circulator = ag.incident_vertices(v), done(circulator);
        double min = Distance(circulator->site(),v->site());
        do{

            weightedDistance = Distance(circulator->site(),v->site());
            if(weightedDistance < min && weightedDistance > TOLERABLYCLOSE){
                min = weightedDistance;
                std::cout << "NO INTERSECTION FOUND: Circulator CONTINUED to next vertex."<< std::endl;

                ClosestVertex = circulator;
            }else if(weightedDistance < 0){
                Intersecting = true;
                ClosestVertex = circulator;
                min = weightedDistance;
                std::cout << "INTERSECTION FOUND: Circulator stopped at intersecting vertex."<< std::endl;
                intersectionCount++;
                break;
            }else if( weightedDistance < TOLERABLYCLOSE){
                Intersecting = false;
                TolerablyBonded = true;
            }else if(weightedDistance == min){
                ClosestVertex = circulator;

            }

           // std::cout << "CtoCirc : " << CenterToCurrentCirculatorSite <<  std::endl;


        }while(++circulator != done);

        Vector_2 CenterToClosest(ClosestVertex->site().point(),v->site().point());
        //K::RT potato = CenterToClosest.squared_length();
        double ClosestVectorDistance = Distance(ClosestVertex->site(),v->site());
        std::cout << "ClosestVectLenght: " << ClosestVectorDistance << std::endl;
        std::cout << "ClosestVectDistance: " << min << std::endl;
        std::cout << "centoclosestVEct: " << CenterToClosest << std::endl;
        Apollonius_site_2 Stay = ClosestVertex->site();
        if(Intersecting == false && ClosestVectorDistance <= TOLERABLYCLOSE && IsIntersecting(ClosestVertex->site(),v->site()) != -1){
            PairBonded++;

            }else if(Intersecting == false){
        Point_2 TranslatePoint = ClosestVertex->site().point() + ((CenterToClosest/CGAL::sqrt(CenterToClosest.squared_length())) * (min*0.9));
        std::cout << "previoussite: " << ClosestVertex->site().point()  << std::endl;
        std::cout << "movedpoint: " <<  TranslatePoint << std::endl;
        Weight TranslateWeight = ClosestVertex->site().weight();
        Apollonius_site_2 translatedSite(TranslatePoint, TranslateWeight);
        end = ag.finite_vertices_end();
        ag.remove(ClosestVertex);

        //QCoreApplication::processEvents();
        //Q_EMIT( changed());
        if(IsIntersecting(translatedSite,v->site()) != -1){
            ag.insert(translatedSite);
            //Q_EMIT( changed());
            end = ag.finite_vertices_end();

            //vit = ag.finite_vertices_begin();
            std::cout << "WE Moved ONE ;-;" <<  std::endl;
            Moved++;




        }else{
            NotReinserted++;
            ag.insert(Stay);
            end = ag.finite_vertices_end();

            std::cout << "WE Stayed ONE ;-;" <<  std::endl;
        }

        }else if(Intersecting == true){
            CenterToClosest = -CenterToClosest;
            Point_2 TranslatePointUnIntersect = ClosestVertex->site().point() + ((CenterToClosest/CGAL::sqrt(CenterToClosest.squared_length())) * (min*1.1));
            Weight TranslateWeightUnIntersect = ClosestVertex->site().weight();
            Apollonius_site_2 translatedSiteUnIntersect(TranslatePointUnIntersect, TranslateWeightUnIntersect);
            Apollonius_site_2 Stay = ClosestVertex->site();
            ag.remove(ClosestVertex);
           // QCoreApplication::processEvents();
            //Q_EMIT( changed());
            if(IsIntersecting(translatedSiteUnIntersect,v->site()) != -1){
                ag.insert(translatedSiteUnIntersect);
                //Q_EMIT( changed());
                end = ag.finite_vertices_end();

                //vit = ag.finite_vertices_begin();
                std::cout << "WE Uninterected ONE ;-;" <<  std::endl;
                Moved++;
            }else{
                NotReinserted++;
                ag.insert(Stay);
                end = ag.finite_vertices_end();

                std::cout << "WE Stayed ONE ;-;" <<  std::endl;
                //std::cout << "WE DROPPED ONE" << std::endl;

            }
        }

        Apollonius::Finite_vertices_iterator vit0;
        vit0 = vit;
        vit0++;
        //vit0++;

        if(vit0 == end && ranVertexIteratorReset == false){
            vit = ag.finite_vertices_begin();
            end = ag.finite_vertices_begin();
            //fit;
            std::cout << "ATTRACTOR RESET RAN" << std::endl;

            ranVertexIteratorReset = true;
        }
    }
    std::cout << "PairsBonded: " << PairBonded <<std::endl;
    std::cout << "Intersection COUNT: " << intersectionCount <<std::endl;
    std::cout <<"Number of vertices: " <<ag.number_of_vertices() << std::endl;


}
 //end attractor


*/


 if (circlesCount != number_of_points){
     std::cout << "Not all circles were inserted, these are left: " << number_of_points - circlesCount << std::endl;
    }
    std::cout << "Done! CirclesInserted:  " << circlesCount << std::endl;
    //std::cout << "Done! Circles NOT REInserted:  " << NotReinserted << std::endl;
    std::cout <<"Number of visible sites: " <<ag.number_of_visible_sites() << std::endl;
    std::cout <<"Number of vertices: " <<ag.number_of_vertices() << std::endl;
    std::cout <<"Number of hidden sites: " <<ag.number_of_hidden_sites() << std::endl;
    //std::cout << "Circles Moved: " << Moved <<std::endl;
    //std::cout << "PairsBonded: " << PairBonded <<std::endl;






    circlesCount = 0;
  // default cursor
  //QApplication::restoreOverrideCursor();
  Q_EMIT( changed());
}


void
MainWindow::on_actionLoadPoints_triggered()
//{
{
  QString fileName = QFileDialog::getOpenFileName(this,
						  tr("Open Points file"),
						  ".");
  std::cout << fileName.toStdString() << std::endl;
  std::string fileLocation = fileName.toStdString();
  QApplication::setOverrideCursor(Qt::WaitCursor);

  std::ifstream inputFileStream(qPrintable(fileName));
  assert(inputFileStream);
  //if(! fileName.isEmpty()){
    Apollonius::Site_2 CircleCenterSite;
    //open(fileName);
    while(inputFileStream >> CircleCenterSite){ //while the file keeps having lines to go through?
        //cout << CircleCenterSite << endl;
        ag.insert(CircleCenterSite);
        //std::cout << "heaaa" << std::endl;
        }

        //validate the Apollonius graph
        assert(ag1.is_valid(true,1));
        std::cout << std::endl;

        QApplication::restoreOverrideCursor();
        Q_EMIT( changed());
  }




void
MainWindow::open(QString fileName)
{
  // wait cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);
  std::ifstream ifs(qPrintable(fileName));
  
  K::Point_2 p;
  std::vector<K::Point_2> points;
  while(ifs >> p) {
    points.push_back(p);
  }
  //  ag.insert(points.begin(), points.end());

  // default cursor
  QApplication::restoreOverrideCursor();
  this->addToRecentFiles(fileName);
  actionRecenter->trigger();
  Q_EMIT( changed());
    
}

void
MainWindow::on_actionSavePoints_triggered()
{

  QString fileName = QFileDialog::getSaveFileName(this,
						  tr("Save points"),
						  ".");
  if(! fileName.isEmpty()){
    std::ofstream ofs(qPrintable(fileName));
    for(Apollonius::Finite_vertices_iterator
          vit = ag.finite_vertices_begin(),
          end = ag.finite_vertices_end();
        vit!= end; ++vit)
    {
      Vertex_handle v = vit;
        ofs << v->site() << std::endl;
    }

    /*for(Apollonius::Finite_faces_iterator ffit = ag.finite_faces_begin(), end = ag.finite_faces_end(); ffit != end; ++ffit){
        Face_handle face = ffit;
        Site_2 site = ag.dual(ffit);

        ofs << site << std::endl;
    }*/
  }



}


void
MainWindow::on_actionRecenter_triggered()
{
  scene.setSceneRect(0, 0, PALLETESIZE, PALLETESIZE);
  QRectF rect(0,0,PALLETESIZE,PALLETESIZE);
  for (int i = 0; i < 100; i++){
      this->graphicsView->setSceneRect(rect);
      this->graphicsView->fitInView(rect, Qt::KeepAspectRatio);
  }

}




#include "Apollonius_graph_2.moc"
#include <CGAL/Qt/resources.h>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  app.setOrganizationDomain("geometryfactory.com");
  app.setOrganizationName("GeometryFactory");
  app.setApplicationName("Apollonius_graph_2 demo");

  // Import resources from libCGAL (Qt5).
  // See http://doc.qt.io/qt-5/qdir.html#Q_INIT_RESOURCE
  CGAL_QT_INIT_RESOURCES;
  Q_INIT_RESOURCE(Apollonius_graph_2);

  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}
