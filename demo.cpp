#include "demo.h"

#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#if defined(ENABLE_QTSCRIPT)
#include <QtScript>
#include <QScriptEngine>
#endif

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "renderwindow.h"
#include "dialog.h"

/*!
 * \brief Constructor
 */
Demo::Demo() {
}

/*!
 * \brief Simple Widget Destructor
 */
Demo::~Demo() {
}


/*!
 * \brief Create a Simple User Interface
 */
void 
Demo::simple() {
    QPushButton* button = new QPushButton(this);
    button->setText("Hello World");
    setCentralWidget(button);
}

/*!
 * \brief Demo to create a signal and attach it to a slot
 */
void
Demo::signalsAndSlots() {
   QGroupBox* box = new QGroupBox(this);
   QVBoxLayout* layout = new QVBoxLayout(box);

   QPushButton* button = new QPushButton();
   button->setText("Open File Dialog Version 1");

   QPushButton* button2 = new QPushButton();
   button2->setText("Open File Dialog Version 2");

   layout->addWidget(button);
   layout->addWidget(button2);

   setCentralWidget(box);

   //connect from signal to slot
   QObject::connect(button, SIGNAL(clicked()), this, SLOT(fileDialogOpened()));

   //connect from signal to signal and then slot
   QObject::connect(button2, SIGNAL(clicked()), SIGNAL(openFileDialog()));
   connect(this, SIGNAL(openFileDialog()), SLOT(fileDialogOpened()));
}

/*!
 * \brief OpenFileDialog Slot
 * Example Open File Dialog Slot that prints selected filename
 */
void
Demo::fileDialogOpened() {
    QString filename = QFileDialog::getOpenFileName(this, "Open File");
    QMessageBox::information(this, "File Selected", filename.length() == 0 ? "No File Selected" : filename);
}


void
Demo::uidemo() {
    Dialog* dialog = new Dialog(this);
    dialog->exec();
}

/*!
 * \brief OpenGL demo
 * Demo highlighting creation of an OpenGL window
 */
void
Demo::opengl() {
    RenderWindowWidget* widget = new RenderWindowWidget(this);
    setCentralWidget(widget);
}


Model*
Demo::parseModel(const QJsonObject& model) {
    Model* out = new Model();

    out->name = model["name"].toString();
    out->fileName = model["filename"].toString();
    QJsonArray jmatrix = model["matrix"].toArray();

    if(jmatrix.size() != 16) {
        qDebug() << "name: " << out->name
                 << " file: " << out->fileName
                 << " matrix length: " << jmatrix.size()
                 << " not 16";
        out->transform.setToIdentity();
    } else {

        float dlist[16];

        for(int i = 0; i < jmatrix.size(); ++i) {
            dlist[i] = (float)jmatrix[i].toDouble();
        }
        out->transform = QMatrix4x4(dlist);
    }
    qDebug() << "load :" << out->name
             << " file: " << out->fileName
             << " transformed " << out->transform;

    return out;
}

Node*
Demo::parseNode(const QJsonObject& parentNode) {
    Node* out = new Node();
    out->type = parentNode["type"].toString();

    /// detected a model
    if(out->type == "model") {
        out->children.push_back(parseModel(parentNode));
    } else if(out->type == "node") {

        QJsonArray nodeArray = parentNode["children"].toArray();
        for(int i = 0; i < nodeArray.size(); ++i) {
            out->children.push_back(parseNode(nodeArray[i].toObject()));
        }
    }

    return out;
}


void
Demo::scenedemoRead(const QString &filename, QVector<Scene*>& scenes) {
    QString fileContents;

    QFile file(filename);

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    fileContents = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileContents.toUtf8());

    QJsonArray sceneList = doc.array();

    for(int i = 0; i < sceneList.size(); ++i) {

        Scene* scene = new Scene();

        QJsonObject object = sceneList[i].toObject();
        scene->name = object["name"].toString();
        scene->description = object["description"].toString();

        QJsonArray rootNode = object["root"].toArray();

        qDebug() << "scene: " << i << " " << scene->name << " " << scene->description;

        for(int i = 0; i < rootNode.size(); ++i) {
            scene->root.push_back(parseNode(rootNode[i].toObject()));
        }
        scenes.push_back(scene);
    }
}


QJsonObject
Demo::writeNode(Node* node) {

    QJsonObject out;
    out["type"] = node->type;

    if(node->type == "model") {

        Model* m = static_cast<Model*>(node->children[0]);

        out["name"] = m->name;
        out["filename"] = m->fileName;

        qDebug() << out["name"] << " " << out["filename"];

        QJsonArray data;
        for(int i = 0; i < 16; ++i) {
            data.push_back((float)m->transform.data()[i]);
        }

        out["matrix"] = data;
    } else if(node->type == "node") {
        QJsonArray children;
        for(int i = 0; i < node->children.size(); ++i) {
            children.push_back(writeNode(node->children[i]));
        }
        out["children"] = children;
    }
    return out;
}

void
Demo::scenedemoWrite(const QString &filename, QVector<Scene *> &scenes) {
    QFile file(filename);

    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QJsonDocument doc;

    QJsonArray sceneList;
    for(int i = 0; i < scenes.size(); ++i) {
        QJsonObject scene;
        scene["name"] = scenes[i]->name;
        scene["description"] = scenes[i]->description;
        qDebug() << "writing : " << scene["name"] << " " << scene["description"];
        QJsonArray rootNode;
        for(int j = 0; j < scenes[i]->root.size(); ++j) {
            QJsonValue value = writeNode(scenes[i]->root[j]);
            rootNode.push_back(value);
        }
        scene["root"] = rootNode;
        sceneList.push_back(scene);
    }

    doc.setArray(sceneList);
    file.write(doc.toJson());
    file.close();

}


void
Demo::scenedemo() {
    QVector<Scene*> scenes;
    scenedemoRead(":/scene/sample", scenes);
    //scenedemoRead("out.json", scenes);
    scenedemoWrite("out2.json", scenes);
}

#if defined(ENABLE_QTSCRIPT)
/// Qt script demo (helper classes)
template <class Container>
QScriptValue toQVariantScriptValue(QScriptEngine *eng, const Container &cont)
{
    QScriptValue a = eng->newVariant(cont);
    return a;
}

template <class Container>
void fromQVariantScriptValue(const QScriptValue &value, Container &cont)
{
    cont = value.toVariant().value<Container>();
}

template <class Container>
QScriptValue toContainerScriptValue(QScriptEngine *eng, const Container &cont)
{
    QScriptValue a = eng->newArray();
    typename Container::const_iterator begin = cont.begin();
    typename Container::const_iterator end = cont.end();
    typename Container::const_iterator it;
    for (it = begin; it != end; ++it)
        a.setProperty(quint32(it - begin), eng->toScriptValue(*it));
    return a;
}

template <class Container>
void fromContainerScriptValue(const QScriptValue &value, Container &cont)
{
    quint32 len = value.property("length").toUInt32();
    for (quint32 i = 0; i < len; ++i) {
        QScriptValue item = value.property(i);
        typedef typename Container::value_type ContainerValue;
        cont.push_back(qscriptvalue_cast<ContainerValue>(item));
    }
}


QVector<QVector4D>
SimpleMath::mult_vec_mat(const QVector<QVector3D>& v, const QMatrix4x4& mat) {

    QVector<QVector4D> out;
    qDebug() << "multiplying vec[i] * matrix";

    for(int i = 0; i < v.size(); ++i) {
        out.push_back(v[i]*mat);
    }
    return out;
}

void
Demo::qtscript() {

    QScriptEngine engine;
    QVector<QVector3D> input_array;
    QVector<QVector4D> output_array;

    QMatrix4x4 matrix;
    matrix.scale(QVector3D(-4,2,1));

    SimpleMath math;

    qScriptRegisterMetaType< QVector<QVector3D> >(&engine, toContainerScriptValue, fromContainerScriptValue);
    qScriptRegisterMetaType< QVector<QVector4D> >(&engine, toContainerScriptValue, fromContainerScriptValue);

    for(int i = 0; i < 3; ++i) {
        input_array.push_back(QVector3D(i,i,i));
        qDebug() << "input = " << input_array.back();
    }

    engine.globalObject().setProperty("array", engine.toScriptValue(input_array));
    engine.globalObject().setProperty("matrix", engine.toScriptValue(matrix));
    engine.globalObject().setProperty("math", engine.newQObject(&math));

    /*
     * example script:
                     "function reverse(array) {  \n"
                     "  for(var i = 0; i < array.length/2; ++i) {\n"
                     "    var temp = array[i]; \n"
                     "    array[i] = array[array.length-i-1]; \n"
                     "    array[array.length-i-1] = temp; \n"
                     "}} \n"
                     "reverse(array);\n"
    */

    QString script = "out = math.mult_vec_mat(array,matrix);\n";
    engine.evaluate(script);

    QScriptValue val = engine.evaluate("out");

    ///copy results back..
    output_array = qscriptvalue_cast< QVector<QVector4D> >(val);
    for(int i = 0; i < output_array.size(); ++i) {
        qDebug() << "output = " << output_array[i];
    }
}
#endif
