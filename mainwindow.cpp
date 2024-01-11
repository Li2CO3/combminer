#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QFont font;
    font.setPixelSize(20);
    font.setFamily("consolas");
    resize(250,90);
    for(int i=0;i<NTHREAD;i++)
    {


        label1[i]=new QLabel(this);
        label1[i]->setGeometry(0+130*(i%2),0+40*(i/2),180,30);
        label2[i]=new QLabel(this);
        label2[i]->setGeometry(0+130*(i%2),20+40*(i/2),180,30);

        label1[i]->setText("");
        label2[i]->setText("");
        label1[i]->show();
        label2[i]->show();
        label1[i]->setFont(font);
        label2[i]->setFont(font);

    }

}

MainWindow::~MainWindow()
{

    FILE *f =fopen("current_progress.txt","w");
    for(int i=0;i<NTHREAD;i++)
    {int length=label1[i]->text().length();
        {fprintf(f,"\"");
        for(int c=0;c<length;c++)
            {QString s=progress[i];char ch=s[c].toLatin1();
            if(ch=='\\'||ch=='\"'||ch=='\'') fprintf(f,"\\");
            fprintf(f,"%c",ch);
        }
        fprintf(f,"\",\n");
        }
    }
    fclose(f);

    for(int i=0;i<NTHREAD;i++)
    {if(label1[i]->text()!="FINISHED") if(c[i])
        c[i]->deleteLater();
     c[i]=NULL;
    }
}

