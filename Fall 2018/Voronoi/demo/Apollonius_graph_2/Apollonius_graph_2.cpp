#include <fstream>
#include <string>
#include <cassert>


// CGAL headers
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Apollonius_graph_hierarchy_2.h>
#include <CGAL/Apollonius_graph_filtered_traits_2.h>
#include <CGAL/point_generators_2.h>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>

// Qt headers
#include <QtGui>
#include <QString>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>

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

typedef CGAL::Apollonius_graph_filtered_traits_2<K,CGAL::Integral_domain_without_division_tag>  Gt;

typedef Gt::Point_2                           Point_2;
typedef K::Circle_2                         Circle_2;
typedef Gt::Site_2                            Apollonius_site_2;
typedef Gt::Site_2::Weight                    Weight;
typedef Gt::Object_2                        Object_2;
typedef Gt::Line_2                          Line_2;


typedef CGAL::Apollonius_graph_2<Gt> Apollonius;
typedef Apollonius::Vertex_handle Vertex_handle;
typedef Apollonius::Face_handle Face_handle;
typedef Apollonius::Site_2 Site_2;


//template function for determining intersection or tangent, returns
template <typename T>
int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

int IsIntersecting(Apollonius::Site_2 &siteA, Apollonius::Site_2 &siteB){

return sign <double> (CGAL::sqrt(CGAL::square(siteA.point().x() - siteB.point().x() )+CGAL::square(siteA.point().y() - siteB.point().y())) - siteA.weight() - siteB.weight());
}

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
  agi->setEdgesPen(QPen(Qt::green, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  //agi->setEdgesPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  scene.addItem(agi);
  //agi->

  // Setup input handlers. They get events before the scene gets them
  // and the input they generate is passed to the triangulation with
  // the signal/slot mechanism

  ci = new CGAL::Qt::GraphicsViewCircleInput<K>(this, &scene);
  QObject::connect(ci, SIGNAL(generate(CGAL::Object)),
		   this, SLOT(processInput(CGAL::Object)));

  scene.installEventFilter(ci);

  //
  // Manual handling of actions
  //

  QObject::connect(this->actionQuit, SIGNAL(triggered()),
		   this, SLOT(close()));

  //
  // Setup the scene and the view
  //
  scene.setItemIndexMethod(QGraphicsScene::NoIndex);
  scene.setSceneRect(0, 0, 2, 2);
  this->graphicsView->setScene(&scene);
  this->graphicsView->setMouseTracking(true);

  // Turn the vertical axis upside down
  //this->graphicsView->matrix().scale(1, -1);
                                                      
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


void
MainWindow::on_actionInsertRandomPoints_triggered()
{

  //QRectF rect = CGAL::Qt::viewportsBbox(&scene);
  QRectF rect(0,0,775,775);
  CGAL::Qt::Converter<K> convert;
  Iso_rectangle_2 isor = convert(rect);
  double width = isor.xmax() - isor.xmin();
  
  CGAL::Random_points_in_iso_rectangle_2<Point_2> pg((isor.min)(), (isor.max)());
  bool ok = false;

  const int number_of_points =
    QInputDialog::getInt(this,
                             tr("Number of random points"),
                             tr("Enter number of random points"),
							 100,
							 0,
							(std::numeric_limits<int>::max)(),
							1,
							&ok);

  if(!ok) {
    return;
  }

  // wait cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);
  std::vector<Apollonius_site_2> points;
  points.reserve(number_of_points);

  double radius1 = 1.0;
  boost::rand48 rng(time(0));
  boost::uniform_real<> dist(0, 1); //radius between 0 and 1
  boost::variate_generator<boost::rand48&, boost::uniform_real<> > radius2(rng,dist); //radius of second species


  boost::random::uniform_int_distribution<> range1(1,1000);
  boost::mt19937 ratioRadius1rng(time(0));
  boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<> > NumberOfRadius1(ratioRadius1rng,range1);
  NumberOfRadius1.distribution().reset();

  boost::random::uniform_int_distribution<> range2(1,1000);
  boost::mt19937 ratioRadius2rng(time(0)+1);
  boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<> > NumberOfRadius2(ratioRadius2rng,range2);
  NumberOfRadius2.distribution().reset();







  std::cout << NumberOfRadius1() << " -- " << NumberOfRadius2() << " -- " <<radius2() << " -- " <<radius1<<  std::endl;

  for(int i = 0; i < number_of_points; ++i){
      //if
    points.push_back(Apollonius_site_2(*pg++,radius2()));
    std::cout << NumberOfRadius1() << " -- " << NumberOfRadius2() << " -- " <<radius2() << " -- " <<radius1<<  std::endl;

  }
      ag.insert(points.begin(), points.end());
  // default cursor
  QApplication::restoreOverrideCursor();
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
        std::cout << endl;

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
    /*for(Apollonius::Finite_vertices_iterator
          vit = ag.finite_vertices_begin(),
          end = ag.finite_vertices_end();
        vit!= end; ++vit)
    {
      Vertex_handle v = vit;
        ofs << v->site() << std::endl;
    }*/

    for(Apollonius::Finite_faces_iterator ffit = ag.finite_faces_begin(), end = ag.finite_faces_end(); ffit != end; ++ffit){
        Face_handle face = ffit;
        Site_2 site = ag.dual(ffit);

        ofs << site << std::endl;
    }
  }



}


void
MainWindow::on_actionRecenter_triggered()
{
  this->graphicsView->setSceneRect(agi->boundingRect());
  this->graphicsView->fitInView(agi->boundingRect(), Qt::KeepAspectRatio);
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
