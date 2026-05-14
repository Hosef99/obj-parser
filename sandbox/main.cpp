#include <GL/glut.h>
#include "parser.h"

float angle = 0.0f;

static OP::Model model;

void init()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); 


    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    model = OP::parse("assets/water_bottle.obj");
}

void apply_material(const OP::Material& mat)
{
    float ambient[] = 
    {
        mat.ambient.x,
        mat.ambient.y,
        mat.ambient.z,
        1.0f
    };

    float diffuse[] = 
    {
        mat.diffuse.x,
        mat.diffuse.y,
        mat.diffuse.z,
        1.0f
    };
    
    float specular[] = 
    {
        mat.specular.x,
        mat.specular.y,
        mat.specular.z,
        1.0f
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat light_pos[] = { 5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glTranslatef(0.0f, 0.0f, -10.0f);

    glRotatef(angle, 1.0f, 1.0f, 0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    for (auto mesh : model.meshes)
    {
        glShadeModel(GL_SMOOTH);
        for (auto sub_mesh : mesh.sub_meshes)
        {
            apply_material(*sub_mesh.material);

            glBegin(GL_TRIANGLES);

            for (auto v : sub_mesh.vertices)
            {
                glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                glTexCoord2f(v.uv.x, v.uv.y);
                glVertex3f(v.position.x, v.position.y, v.position.z);
            }

            glEnd();
        }
    }

    glutSwapBuffers();
}

void update(int value)
{
    angle += 1.0f;

    if (angle > 360.0f)
        angle -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;

    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, aspect, 0.1, 100.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Hello Cube");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}
