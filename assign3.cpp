#include<iostream>
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <list>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include<stdlib.h>
#include <stddef.h>
#include <map>
#include <cmath>
using namespace std;
#define PINF 65535
#define NINF -65535
int global_turn=0;
FILE *opf=NULL;
FILE *opl=NULL;
class Vertex;

class Edge
{
	public:
		Edge(Vertex *org, Vertex *dest)
		{
			origin = org;
			destination = dest;
		}

		Vertex* getOrigin() {return origin;}
		Vertex* getDestination() {return destination;}
	private:
		Vertex* origin;
		Vertex* destination;
};


class Vertex
{
	public:
		float cost;
		bool explored;
		//Vertex *parent;
		int depth;
		int group;
		int temp_group;
		int commit_group;
		Vertex(string id,float cost,int group)
		{
			depth=-1;
			//parent=NULL;
			explored=false;
			name = id;
			this->cost=cost;
			this->group=group;
			this->temp_group=group;
		}

		void addEdge(Vertex *v)
		{
			Edge newEdge(this, v);
			edges.push_back(newEdge);
		}
		std::list<string> getFreeNeihbor(int &len)
		{
			int j=0;
			std::list<string> adj;
			//char *adj=new char[30];
			for (int i = 0; i < edges.size(); i++)
			{
				Edge e = edges[i];
				//cout<<"Neihbor "<<e.getOrigin()->getName()<<"-->"<<e.getDestination()->getName()[0]<<endl;
				if(e.getDestination()->group == 0)
				{
					adj.push_back(e.getDestination()->getName());
					//adj[j]= e.getDestination()->getName()[0];
				//	cout<<"Free ones-->"<<adj[j]<<endl;  	
					j++;
				}
			}
			len=j;
			return adj;
		}
		int opponentNeighborCost(int groupl)
		{
		float c=0;
		for (int i = 0; i < edges.size(); i++)
                        {
                                Edge e = edges[i];
                                if(e.getDestination()->group == groupl)
                                {
					//cout<<"\ngroupl\t"<<e.getDestination()->group<<","<<groupl;

                                        c=c+ e.getDestination()->cost;
                                }
                        }
		//cout<<"\nOpponent's Neihbor cost -->"<<c;
		return c;

		}

		void captureOpponent()
		{
		//cout<<"\ncapture"<<!(this->group)<<endl;
		int groupl=0;
		if(this->group == 1)
		 groupl=-1;
		else if (this->group == -1)
		 groupl=1;
                for (int i = 0; i < edges.size(); i++)
                        {
                                Edge e = edges[i];
                                if(e.getDestination()->group == groupl)
                                {
				e.getDestination()->group=this->group;
                                }
                        }

		}

		void printEdges()
		{
			cout << name << ":" << endl;
			for (int i = 0; i < edges.size(); i++)
			{
				Edge e = edges[i];
				cout <<e.getOrigin()->getName()<<"-->"<< e.getDestination()->getName() << endl;
			}
			cout << endl;
		}

		string getName() {return name;} 
		vector<Edge> getEdges() {return edges;}

		string name;
		vector<Edge> edges;
};
class obj
{
	public:
	float cost;
	int group;
	int dest;
	int action;
	float alpha;
	float beta;
};

struct CompareNode : public std::binary_function<obj,obj,bool>
{
	bool operator()(obj lhs, obj rhs) const
	{
		return lhs.cost < rhs.cost;
	}
};

class Graph
{
	public:
		Graph() {
			this->depth_terminal=0;
			alpha_stat=-65535;
			}

		void insert(Vertex *v)
		{
			vertices.push_back(v);
		}

		void printGraph()
		{
			for (int i = 0; i < vertices.size(); i++)
				vertices[i]->printEdges();
		}
		void reset()
		{
			for (int i = 0; i < vertices.size(); i++)
			{
				vertices[i]->explored=false;
				//	vertices[i]->parent=NULL;
			}
		}

		int getIndex(string nodename)
		{
			for(int i = 0; i < vertices.size(); i++)
			{
				//cout<<vertices[i]->name[0]<<"getInd"<<endl;
				if(vertices[i]->name ==nodename)
				{
					return i; 
				}
			}
			return -1;

		}
		void print_glog(obj ob)
		{
		fprintf(opl,"Union,");
		if(ob.action==0)
			{
			fprintf(opl,"Force March,");
			}
		else
			{
			fprintf(opl,"Paratroop Drop,");
			}
		fprintf(opl,"%s,1,%.1f\n",vertices[ob.dest]->getName().c_str(),ob.cost);

		}
		 void print_minmaxlog(int dest=-1,float value=-1,int action=-1,int player=-1,int depth=-1)
                {
		if(global_turn == 0)
		{
		if(dest == -1)
			{
			fprintf(opl,"N/A,N/A,N/A,0,");
				if(value <= -30000)
					{
					fprintf(opl,"-Infinity\n");
					}
				else if(value >= 30000)
					{
					fprintf(opl,"Infinity\n");
					}
				else
					{
					fprintf(opl,"%.1f\n",value);
					}
			}
		else
		{
		if(player == 1)
			{
			fprintf(opl,"Union,");
			}
		else
			{
			fprintf(opl,"Confederacy,");
			}	
                if(action==0)
                        {
                        fprintf(opl,"Force March,");
                        }
                else
                        {
                        fprintf(opl,"Paratroop Drop,");
                        }
                fprintf(opl,"%s,%d,",vertices[dest]->getName().c_str(),depth);
			if(value <= -30000)
                                        {
					  fprintf(opl,"-Infinity\n");
                                        }
                                else if(value >= 30000)
                                        {
					  fprintf(opl,"Infinity\n");
                                        }
                                else
                                        {
					  fprintf(opl,"%.1f\n",value);
                                        }


		}
		}
                }
		void print_prunedlog(int dest=-1,float value=-1,int action=-1,int player=-1,int depth=-1,float alpha=-1,float beta=-1,int cutoff=-1)
                {
		if(global_turn == 0)
		{
		if(dest == -1)
			{
			  fprintf(opl,"N/A,N/A,N/A,0,");
				if(value <= -30000)
					{
					  fprintf(opl,"-Infinity,");
					}
				else if(value >= 30000)
					{
					  fprintf(opl,"Infinity,");
					}
				else
					{
					  fprintf(opl,"%.1f,",value);
					}

				if(alpha <= -30000)
                                        {
					  fprintf(opl,"-Infinity,");
                                        }
                                else if(alpha >= 30000)
                                        {
					  fprintf(opl,"Infinity,");
                                        }
                                else
                                        {
					  fprintf(opl,"%.1f,",alpha);
                                        }

				if(beta <= -30000)
                                        {
					  fprintf(opl,"-Infinity");
                                        }
                                else if(beta >= 30000)
                                        {
					  fprintf(opl,"Infinity");
                                        }
                                else
                                        {
					  fprintf(opl,"%.1f",beta);
                                        }

			}
		else
		{
		if(player == 1)
			{
			  fprintf(opl,"Union,");
			}
		else
			{
			  fprintf(opl,"Confederacy,");
			}	
                if(action==0)
                        {
			  fprintf(opl,"Force March,");
                        }
                else
                        {
			  fprintf(opl,"Paratroop Drop,");
                        }
                fprintf(opl,"%s,%d,",vertices[dest]->getName().c_str(),depth);
			if(value <= -30000)
                                        {
					  fprintf(opl,"-Infinity,");
                                        }
                                else if(value >= 30000)
                                        {
					  fprintf(opl,"Infinity,");
                                        }
                                else
                                        {
					  fprintf(opl,"%.1f,",value);
                                        }

			 if(alpha <= -30000)
                                        {
					  fprintf(opl,"-Infinity,");
                                        }
                                else if(alpha >= 30000)
                                        {
					  fprintf(opl,"Infinity,");
                                        }
                                else
                                        {
					  fprintf(opl,"%.1f,",alpha);
                                        }

                                if(beta <= -30000)
                                        {
					  fprintf(opl,"-Infinity");
                                        }
                                else if(beta >= 30000)
                                        {
					  fprintf(opl,"Infinity");
                                        }
                                else
                                        {
					  fprintf(opl,"%.1f",beta);
                                        }



		}
		if(cutoff == 1)
			{
			  fprintf(opl,",CUT-OFF\n");
			}
		else
			{
			  fprintf(opl,"\n");
			}
		}
                }



		std::list<int> fmpdCandidates(int groupl)
		{
			//int fmpdlist=new int[30];
			std::list<int> fmlist;
			int k=0;
			int len=0;
			for(int i = 0; i < vertices.size(); i++)
			{
				//cout<<"In fmpd"<<endl;
				if(vertices[i]->group == groupl)
				{
					//cout<<"\nInside";	
					std::list <string>adj=vertices[i]->getFreeNeihbor(len);
					
for (std::list<string>::iterator it=adj.begin(); it!=adj.end(); ++it)
	fmlist.push_back(getIndex(*it));
				} 		
			}
			fmlist.sort();
			fmlist.unique();
			//fmpdlist.unique(same_int);	
			return fmlist; 
		}


		std::list<int> pdCandidates(int group)
		{
			std::list<int> pdlist;
			//int *pdlist=new int[30];
			int k=0;
			for(int i = 0; i < vertices.size(); i++)
			{
				if(vertices[i]->group == 0)
				{
					//	pdlist[k]=i;
					pdlist.push_back(i);
					//	k++;		
				} 
			}
			pdlist.sort();
			pdlist.unique();
			return pdlist; 
		}
		float Eval(int groupl)
		{
		float A=0,B=0;
		for(int i = 0; i < vertices.size(); i++)
                        {
			 if(vertices[i]->group == 1)
                                {
				A=A+vertices[i]->cost;
				}
			 if(vertices[i]->group == -1)
                                {
                                B=B+vertices[i]->cost;
                                }
			}

		if(groupl == 1)
			{
			return A-B;
			}
		else if(groupl == -1)
			{
			return B-A;
			}
	
		}
		bool gameend()
		{
		int flag=0;
	         for(int i = 0; i < vertices.size(); i++)
                        {
                                if(vertices[i]->group == 0)
                                {
				flag=1;
                                }
                        }
		if(flag == 1)
			return false;
		else
			return true;	
 		
	         	
		}
		void print_path(int dest=-1,int action=-1,int group=0)
		{
		static int turn=0;
		fprintf(opf,"\nTURN = %d\n",turn);
		//cout<<"dest "<<dest<<endl;
		if(turn ==0)
			{
			fprintf(opf,"Player = N/A\n");
			fprintf(opf,"Action = N/A\n");
			fprintf(opf,"Destination = N/A\n");
			}
		else
			{
			if (group==1)
			fprintf(opf,"Player =Union\n");
			else
			fprintf(opf,"Player =Confederacy\n");
			if(action ==0)
			fprintf(opf,"Action = Force March\n");
			else
			fprintf(opf,"Action = Paratroop Drop\n");

			fprintf(opf,"Destination =%s\n",vertices[dest]->getName().c_str());
			}
		turn++;
		float cost=0;
		fprintf(opf,"Union ,{");
		 for(int i = 0; i < vertices.size(); i++)
                        {
                                if(vertices[i]->group == 1)
                                {
				fprintf(opf,"%s,",vertices[i]->getName().c_str());
				cost=cost+vertices[i]->cost;
				}
			}
		fprintf(opf,"},%.1f\n",cost);
		cost=0;	
		fprintf(opf,"Confederacy ,{");
                 for(int i = 0; i < vertices.size(); i++)
                        {
                                if(vertices[i]->group == -1)
                                {
				fprintf(opf,"%s,",vertices[i]->getName().c_str());	
                               // cout<<vertices[i]->getName();
				cost=cost+vertices[i]->cost;	
                                }
                        }
                fprintf(opf,"},%.1f\n",cost);
	
		fprintf(opf,"---------------------------------\n");	

		}
		void greedy()
		{
		int first_step=1;
		std::list<int> fm;
		std::list<int> pd;
		obj ob;
		obj temp;
		float es=Eval(1);
		print_path();
			while(1)
			{
		es=Eval(1);
		 priority_queue<obj ,std::vector<obj >,CompareNode > pq;
			fm=fmpdCandidates(1);
		//cout<<"\nUnion es\t"<<es;
		//cout<<"\nfm Candidates";
	for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
		{
				obj ob;
				ob.cost=es+vertices[*it]->opponentNeighborCost(-1)+vertices[*it]->cost+vertices[*it]->opponentNeighborCost(-1);
				//cout<<"ob cost "<<ob.cost<<endl;
				ob.dest=*it;
				ob.group=1;
				ob.action=0;
				if(first_step == 1)
					{
					print_glog(ob);
					}
				pq.push(ob);
		}


				pd=pdCandidates(1);
			//cout<<"\npd Candidates";	
        for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
				obj ob;
                                ob.cost=es+vertices[*it]->cost;
                                ob.dest=*it;
                                ob.group=1;
                                ob.action=1;
				if(first_step == 1)
                                        {
                                        print_glog(ob);
                                        }
	
				//cout<<"ob=="<<vertices[ob.dest]->getName()<<","<<ob.cost<<endl;
                                pq.push(ob);
                }
	first_step=0;
	
//	cout<<"\n And the Winner is \n";
	temp=pq.top();
//	pq.pop();
	//cout<<"Dest -->"<<temp.dest<<",action -->"<<temp.cost<<"\t"<<pq.top().cost;

	priority_queue<obj ,std::vector<obj >,CompareNode > pq1;
	vertices[temp.dest]->group=1;
	if(temp.action == 0)
		{
		vertices[temp.dest]->captureOpponent();
		}
	 print_path(temp.dest,temp.action,temp.group);			
				if(gameend())
					break;
		es=Eval(-1);
		//cout<<"\nConf es"<<es;
				fm=fmpdCandidates(-1);

 for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
                                ob.cost=es+vertices[*it]->opponentNeighborCost(1)+vertices[*it]->cost+vertices[*it]->opponentNeighborCost(1);
                                ob.dest=*it;
                                ob.group=-1;
                                ob.action=0;
				//cout<<"\nob=="<<vertices[ob.dest]->getName()<<","<<ob.cost<<endl;
                                pq1.push(ob);
                }



				pd=pdCandidates(-1);
		for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
                                ob.cost=es+vertices[*it]->cost;
                                ob.dest=*it;
                                ob.group=-1;
                                ob.action=1;
				//cout<<"\nob=="<<vertices[ob.dest]->getName()<<","<<ob.cost<<endl;
                                pq1.push(ob);
                }


		//cout<<"\n And the Winner is \n";
        temp=pq1.top();
//	pq1.pop();
       //cout<<"Dest1 -->"<<temp.dest<<",action -->"<<temp.cost<<"\t"<<pq1.top().cost; 
	vertices[temp.dest]->group=-1;
        if(temp.action == 0)
                {
                vertices[temp.dest]->captureOpponent();
                }

	print_path(temp.dest,temp.action,temp.group);
				if(gameend())
					break;
			}

		}

obj MaxValue(int vertex,int actionl)
            {
	obj temp;
//print_minmaxlog(vertex,value,actionl);
            std::list<int> fm;
            std::list<int> pd;
            int commitgroup[300];
	    static int print_flag=2;

            this->depth_terminal++;
            //Save State
            for(int i=0;i<vertices.size();i++)
                {
                commitgroup[i]=vertices[i]->group;
                }

            vertices[vertex]->group=-1;
            if(actionl ==0) // Force March? Capture neihbors
                {
                vertices[vertex]->captureOpponent();
                }
	  //cout<<"depth_term "<<this->depth_terminal<<"\tterminal "<<terminal<<endl;
            if((this->depth_terminal== terminal) || (gameend() ))
                {
		temp.cost=Eval(1);
		//cout<<"\ntemp,cost "<<temp.cost<<endl;
		temp.dest=vertex;
		temp.action=actionl;
		 //resetState();

                for(int i=0;i<vertices.size();i++)
                        {
                        vertices[i]->group=commitgroup[i];
                        }

		//print_path(temp.dest,temp.action,temp.group);
		print_minmaxlog(vertex,temp.cost,actionl,-1,this->depth_terminal);
		this->depth_terminal--;
                return temp;
                }
            //Apply the Action//Make Conf Moves 
           // saveState();

            float value=-65535;

          fm=fmpdCandidates(1);
          pd=pdCandidates(1);
		print_flag=2;
//print_minmaxlog(vertex,value,actionl);
          for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
		if(print_flag !=1)
			print_minmaxlog(vertex,value,actionl,-1,this->depth_terminal);
		print_flag++;
                value=max(value,MinValue(*it,0).cost);
		print_minmaxlog(vertex,value,actionl,-1,this->depth_terminal);
		print_flag=1;
                }

	for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
		if(print_flag !=1)
			print_minmaxlog(vertex,value,actionl,-1,this->depth_terminal);
		print_flag++;
                value=max(value,MinValue(*it,1).cost);
		print_minmaxlog(vertex,value,actionl,-1,this->depth_terminal);
		print_flag=1;
                }

           //resetState();

          for(int i=0;i<vertices.size();i++)
              {
                vertices[i]->group=commitgroup[i];
              }
	  temp.cost=value;
	this->depth_terminal--;
               temp.dest=vertex;
                temp.action=actionl;
	//print_path(temp.dest,temp.action,temp.group);
	//print_minmaxlog(vertex,value,actionl);
          return temp;



}
obj Pruned_MaxValue(int vertex,int actionl,float alpha=-65535,float beta=65535)
            {
        obj temp;

            std::list<int> fm;
            std::list<int> pd;
            int commitgroup[300];
		static int print_flag=2;
            this->depth_terminal++;
            //Save State
            for(int i=0;i<vertices.size();i++)
                {
                commitgroup[i]=vertices[i]->group;
                }

            vertices[vertex]->group=-1;
            if(actionl ==0) // Force March? Capture neihbors
                {
                vertices[vertex]->captureOpponent();
                }
          //cout<<"depth_term "<<this->depth_terminal<<"\tterminal "<<terminal<<endl;
            if((this->depth_terminal== terminal) || (gameend() ))
                {
                temp.cost=Eval(1);
                temp.dest=vertex;
                temp.action=actionl;
		temp.alpha=alpha;
		temp.beta=beta;
                 //resetState();

                for(int i=0;i<vertices.size();i++)
                        {
                        vertices[i]->group=commitgroup[i];
                        }

                //print_path(temp.dest,temp.action,temp.group);

		print_prunedlog(temp.dest,temp.cost,actionl,-1,this->depth_terminal,alpha,beta);
		this->depth_terminal--;

                return temp;
                }
            //Apply the Action//Make Conf Moves 
           // saveState();
	 float value=-65535;

          fm=fmpdCandidates(1);
          pd=pdCandidates(1);
		print_flag=2;
          for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
	if(print_flag != 1)
		print_prunedlog(vertex,value,actionl,-1,this->depth_terminal,alpha,beta);
	print_flag++;
                value=max(value,Pruned_MinValue(*it,0,alpha,beta).cost);
		 alpha=max(alpha,value);
                temp.alpha=alpha;
                temp.beta=beta;

		if(value >= beta)
			{
			temp.cost=value;
               		temp.dest=vertex;
                	temp.action=actionl;
			for(int i=0;i<vertices.size();i++)
              			{
                		vertices[i]->group=commitgroup[i];
              			}
			cout<<"\nCut-off";
			print_prunedlog(temp.dest,temp.cost,actionl,-1,this->depth_terminal,alpha,beta,1);
			this->depth_terminal--;
			temp.alpha=alpha;
                	temp.beta=beta;

			return temp;

			}
	//	alpha=max(alpha,value);
	//	temp.alpha=alpha;
         //       temp.beta=beta;
		print_prunedlog(vertex,value,actionl,-1,this->depth_terminal,alpha,beta);
		print_flag=1;
                }


        for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
		if(print_flag !=1)
		print_prunedlog(vertex,value,actionl,-1,this->depth_terminal,alpha,beta);
		
		print_flag++;
                value=max(value,Pruned_MinValue(*it,1,alpha,beta).cost);
		alpha=max(alpha,value);
                temp.alpha=alpha;
                temp.beta=beta;

		if(value >= beta)
			{
			temp.cost=value;
			temp.dest=vertex;
		print_prunedlog(temp.dest,temp.cost,actionl,-1,this->depth_terminal,alpha,beta,1);
                        this->depth_terminal--;
                        temp.action=actionl;
			for(int i=0;i<vertices.size();i++)
                                {
                                vertices[i]->group=commitgroup[i];
                                }
			temp.alpha=alpha;
                        temp.beta=beta;


                        return temp;
			}
		
               // alpha=max(alpha,value);
	//	temp.alpha=alpha;
         //       temp.beta=beta;
		print_prunedlog(vertex,value,actionl,-1,this->depth_terminal,alpha,beta);
		print_flag=1;
                }

           //resetState();

          for(int i=0;i<vertices.size();i++)
              {
                vertices[i]->group=commitgroup[i];
              }
          temp.cost=value;
        this->depth_terminal--;
               temp.dest=vertex;
                temp.action=actionl;
	temp.alpha=alpha;
        temp.beta=beta;

        //print_path(temp.dest,temp.action,temp.group);
          return temp;

}

	    obj MinValue(int vertex,int actionl)
	    {
	obj temp;
	    
            std::list<int> fm;
            std::list<int> pd;
	    int commitgroup[300];
		static int print_flag=2;

	    this->depth_terminal++;
	    //Save State
	    for(int i=0;i<vertices.size();i++)
		{
		commitgroup[i]=vertices[i]->group;
		}

            vertices[vertex]->group=1;
	    if(actionl ==0) // Force March? Capture neihbors
		{
		vertices[vertex]->captureOpponent();
		}
	    //cout<<"depth_term "<<this->depth_terminal<<"\tterminal "<<terminal<<endl;
	    if((this->depth_terminal== terminal) || (gameend() ))
		{
		temp.cost=Eval(1);
		//cout<<"\ntemp.cost "<<temp.cost<<endl;
		//this->depth_terminal--;
		 //resetState();

          	for(int i=0;i<vertices.size();i++)
              		{
                	vertices[i]->group=commitgroup[i];
              		}
                temp.dest=vertex;
                temp.action=actionl;
		//print_path(temp.dest,temp.action,temp.group);

		print_minmaxlog(vertex,temp.cost,actionl,1,this->depth_terminal);
		this->depth_terminal--;
		return temp;		
		}
	    //Apply the Action//Make Conf Moves 
	   // saveState();
	   
	   float value=65535;
	//print_minmaxlog(vertex,value,actionl);
	  fm=fmpdCandidates(-1);
	  pd=pdCandidates(-1);
	print_flag=2;
	  for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
		if(print_flag != 1)
			print_minmaxlog(vertex,value,actionl,1,this->depth_terminal);
		print_flag++;
		value=min(value,MaxValue(*it,0).cost);
		print_minmaxlog(vertex,value,actionl,1,this->depth_terminal);
		print_flag=1;
		}
	

	  for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
		if(print_flag != 1)
			print_minmaxlog(vertex,value,actionl,1,this->depth_terminal);
		print_flag++;
		//cout<<"\nvalue "<<value<<endl;
		value=min(value,MaxValue(*it,1).cost);
		//cout<<"\nvalue "<<value<<endl;
		print_minmaxlog(vertex,value,actionl,1,this->depth_terminal);
		print_flag=1;
		}

	
	   //resetState();
	   
	  for(int i=0;i<vertices.size();i++)
              {
                vertices[i]->group=commitgroup[i];
              }
	temp.cost=value;
	this->depth_terminal--;
                temp.dest=vertex;
                temp.action=actionl;
	//print_path(temp.dest,temp.action,temp.group);
//	print_minmaxlog(vertex,value,actionl);
	  return temp;	

	    }
	 obj Pruned_MinValue(int vertex,int actionl,float alpha,float beta)
            {
        obj temp;
            std::list<int> fm;
            std::list<int> pd;
            int commitgroup[300];
		static int print_flag=2;
            this->depth_terminal++;
            //Save State
            for(int i=0;i<vertices.size();i++)
                {
                commitgroup[i]=vertices[i]->group;
                }

            vertices[vertex]->group=1;
            if(actionl ==0) // Force March? Capture neihbors
                {
                vertices[vertex]->captureOpponent();
                }
            //cout<<"depth_term "<<this->depth_terminal<<"\tterminal "<<terminal<<endl;
            if((this->depth_terminal== terminal) || (gameend() ))
                {
                temp.cost=Eval(1);
		print_prunedlog(vertex,temp.cost,actionl,1,this->depth_terminal,alpha,beta);
                //this->depth_terminal--;
                 //resetState();

                for(int i=0;i<vertices.size();i++)
                        {
                        vertices[i]->group=commitgroup[i];
                        }
                temp.dest=vertex;
                temp.action=actionl;
		if((this->alpha_stat<temp.cost) && (this->depth_terminal == 1))
			{
			this->alpha_stat=temp.cost;
			}
		this->depth_terminal--;
		temp.alpha=alpha;
                temp.beta=beta;

                //print_path(temp.dest,temp.action,temp.group);
                return temp;
                }
            //Apply the Action//Make Conf Moves 
           // saveState();

            float value=65535;
	  fm=fmpdCandidates(-1);
          pd=pdCandidates(-1);
	
	  print_flag=2;

          for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
		if(print_flag !=1)
		print_prunedlog(vertex,value,actionl,1,this->depth_terminal,alpha,beta);
	
		print_flag++;	
                value=min(value,Pruned_MaxValue(*it,0,alpha,beta).cost);
		beta=min(beta,value);
		temp.alpha=alpha;
                temp.beta=beta;

		if(value<=alpha)
			{
			 for(int i=0;i<vertices.size();i++)
		              {
               			 vertices[i]->group=commitgroup[i];
              		      }
			if((this->alpha_stat<value)&&((this->depth_terminal == 1)))
                        {
                       // this->alpha_stat=value;
                        }

        		temp.cost=value;
			print_prunedlog(vertex,value,actionl,1,this->depth_terminal,alpha,beta,1);
        		this->depth_terminal--;
                	temp.dest=vertex;
                	temp.action=actionl;


			return temp;
			}
	//	beta=min(beta,value);
		if((this->alpha_stat<value) &&(this->depth_terminal == 1))
                        {
                        this->alpha_stat=value;
                        }

		print_prunedlog(vertex,value,actionl,1,this->depth_terminal,alpha,beta);
		print_flag=1;
                }


          for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
		if(print_flag !=1)
		  print_prunedlog(vertex,value,actionl,1,this->depth_terminal,alpha,beta);
		print_flag++;
                value=min(value,Pruned_MaxValue(*it,1,alpha,beta).cost);
		beta=min(beta,value);
		temp.alpha=alpha;
                temp.beta=beta;
	
		if(value<=alpha)
			 {
                         for(int i=0;i<vertices.size();i++)
                              {
                                 vertices[i]->group=commitgroup[i];
                              } 

			if((this->alpha_stat<value)&&(this->depth_terminal == 1))
                        {
                        //this->alpha_stat=value;
                        }

                        temp.cost=value;
			print_prunedlog(vertex,value,actionl,1,this->depth_terminal,alpha,beta,1);
                        this->depth_terminal--;
                        temp.dest=vertex;
                        temp.action=actionl;
			temp.alpha=alpha;
                        temp.beta=beta;

                        return temp;
                        }

               // beta=min(beta,value);
		if((this->alpha_stat<value)&&(this->depth_terminal == 1))
                        {
                        this->alpha_stat=value;
                        }

		print_prunedlog(vertex,value,actionl,1,this->depth_terminal,alpha,beta);
		print_flag=1;
                }

           //resetState();

          for(int i=0;i<vertices.size();i++)
              {
                vertices[i]->group=commitgroup[i];
              }
        temp.cost=value;
        this->depth_terminal--;
                temp.dest=vertex;
                temp.action=actionl;
		temp.alpha=alpha;
                temp.beta=beta;

        //print_path(temp.dest,temp.action,temp.group);
          return temp;

        }

            //Returns obj with Vertex Number
	    obj greedy_move()
	    {

		std::list<int> fm;
                std::list<int> pd;
                obj ob;
                obj temp;
                 
                priority_queue<obj ,std::vector<obj >,CompareNode > pq1;


		float es=Eval(-1);
	
                //cout<<"\nConf es"<<es;
                                fm=fmpdCandidates(-1);

 for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
                                ob.cost=es+vertices[*it]->opponentNeighborCost(1)+vertices[*it]->cost+vertices[*it]->opponentNeighborCost(1);
                                ob.dest=*it;
                                ob.group=-1;
                                ob.action=0;
                                //cout<<"\nob=="<<vertices[ob.dest]->getName()<<","<<ob.cost<<endl;
                                pq1.push(ob);
                }



                                pd=pdCandidates(-1);
                for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
                                ob.cost=es+vertices[*it]->cost;
                                ob.dest=*it;
                                ob.group=-1;
                                ob.action=1;
                                //cout<<"\nob=="<<vertices[ob.dest]->getName()<<","<<ob.cost<<endl;
                                pq1.push(ob);
                }


                //cout<<"\n And the Winner is \n";
        temp=pq1.top();
//      pq1.pop();
       //cout<<"Dest1 -->"<<temp.dest<<",action -->"<<temp.cost<<"\t"<<pq1.top().cost; 
        vertices[temp.dest]->group=-1;
        if(temp.action == 0)
                {
                vertices[temp.dest]->captureOpponent();
                }
	
	return temp;

	    }
	    //Return obj with Vertex Number
	    obj Minimax_move()
	    {
	     //For all candidates of union return Max(MinValue())
		std::list<int> fm;
                std::list<int> pd;
		obj max;
		float cost=-65535;
		static int print_flag=2;
		std::list<obj> templist;
		obj temp;
		temp.group=1;
		temp.cost=-65535;
		max.cost=-65535;	
		fm=fmpdCandidates(1);
		print_flag=2;
		for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
		this->depth_terminal=0;
		if(print_flag != 1)
			print_minmaxlog(-1,cost);
		print_flag++;
		temp=MinValue(*it,0);//pass Vertex number
		temp.action=0;
		templist.push_back(temp);
		if(cost<temp.cost)
			cost=temp.cost;
		print_minmaxlog(-1,cost);
		print_flag=1;	
		}
		
		pd=pdCandidates(1);
		for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
		this->depth_terminal=0;
		if(print_flag != 1)
			print_minmaxlog(-1,cost);
		print_flag++;
		temp=MinValue(*it,1);//pass Vertex number
		temp.action=1;
                templist.push_back(temp);
		if(cost<temp.cost)
                        cost=temp.cost;

		print_minmaxlog(-1,cost);
		print_flag=1;
		}
	   //Find the Max in templist and return it 
    for (std::list<obj>::iterator it=templist.begin(); it!=templist.end(); ++it)
                {
		if(max.cost <(*it).cost)
		   {
			//cout<<"\n*itcost "<<(*it).cost;
		   max.cost=(*it).cost;
		   max=*it;
		   }
		
		}


	 vertices[max.dest]->group=1;
        if(max.action == 0)
                {
                vertices[max.dest]->captureOpponent();
                }
	max.group=1;
	global_turn++;
	 return max;

	    
	    }

	obj Pruned_Minimax_move()
            {
             //For all candidates of union return Max(MinValue())
                std::list<int> fm;
                std::list<int> pd;
                obj max;
		static int print_flag=2;
		print_flag=2;	
                std::list<obj> templist;
                obj temp;
		float max_alpha=-65535;
                temp.group=1;
                max.cost=-65535;
		//temp.alpha=-65535;
		//temp.beta=65535;
                fm=fmpdCandidates(1);
		static int i=1;
		i=1;

                for (std::list<int>::iterator it=fm.begin(); it!=fm.end(); ++it)
                {
                this->depth_terminal=0;
		if(i==1)
			{
			print_prunedlog(-1,-65535,-1,-1,0,-65535,65535);

			temp=Pruned_MinValue(*it,0,-65535,65535);//pass Vertex number
			i++;
			}
		else
			{
			if(max_alpha<temp.cost)
				max_alpha=temp.cost;
			if(print_flag != 1)
				print_prunedlog(-1,max_alpha,-1,-1,0,max_alpha,65535);
			print_flag++;
                	temp=Pruned_MinValue(*it,0,max_alpha,65535);//pass Vertex number
			if(max_alpha<temp.cost)
                                max_alpha=temp.cost;

			print_prunedlog(-1,max_alpha,-1,-1,0,max_alpha,65535);
			print_flag=1;
			//cout<<"\ncoss "<<temp.cost<<endl;
			}
                temp.action=0;
                templist.push_back(temp);
                }

                pd=pdCandidates(1);
                for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
                {
                this->depth_terminal=0;
		if(i==1)
                        {
                          print_prunedlog(-1,max_alpha,-1,-1,0,max_alpha,65535);
	
                        temp=Pruned_MinValue(*it,1,-65535,65535);//pass Vertex number
                       i++; 
                        }
                else
                        {
			if(max_alpha<temp.cost)
                                max_alpha=temp.cost;
			if(print_flag != 1)
                                print_prunedlog(-1,max_alpha,-1,-1,0,max_alpha,65535);
			print_flag++;
                        temp=Pruned_MinValue(*it,1,max_alpha,65535);//pass Vertex number
			if(max_alpha<temp.cost)
                                max_alpha=temp.cost;
			print_prunedlog(-1,max_alpha,-1,-1,0,max_alpha,65535);
			print_flag=1;
                        }

                temp.action=1;
                templist.push_back(temp);

                }
           //Find the Max in templist and return it 
    for (std::list<obj>::iterator it=templist.begin(); it!=templist.end(); ++it)
                {
                if(max.cost <(*it).cost)
                   {
                        cout<<"\n*itcost "<<(*it).cost;
                   max.cost=(*it).cost;
                   max=*it;
                   }

                }

	 vertices[max.dest]->group=1;
        if(max.action == 0)
                {
                vertices[max.dest]->captureOpponent();
                }
        max.group=1;
	global_turn++;
         return max;

	}
		
	    //Alternates between Minimax move and greedy move
	    void Minimax_game()
	    {
	   print_path();
	    while(1)
	    	{
	    	obj Union=Minimax_move();
		//vertices[Union.dest]->group=1;
		if(gameend())
		 break;	
		cout<<"\nChecking Minimax move == max"<<Union.cost<<"\n";
		print_path(Union.dest,Union.action,Union.group);
	    	obj Conf=greedy_move();
		//vertices[Conf.dest]->group=-1;
		print_path(Conf.dest,Conf.action,Conf.group);	
		if(gameend())
		 break;	
	    	}

	    }
	void Pruned_game()
	{
	 print_path();
            while(1)
                {
                obj Union=Pruned_Minimax_move();
                //vertices[Union.dest]->group=1;
                if(gameend())
                 break;
                cout<<"\nChecking Minimax move == max"<<Union.cost<<"\n";
                print_path(Union.dest,Union.action,Union.group);
                obj Conf=greedy_move();
                //vertices[Conf.dest]->group=-1;
                print_path(Conf.dest,Conf.action,Conf.group);
                if(gameend())
                 break;
                }

	}
	   
		int terminal;
	private:
		vector<Vertex*> vertices;
		int depth_terminal;
		int alpha_stat;
};

main(int argc,char *argv[])
{


int task;
int depth;
char mapname[50];
char initmap[50];
char outputpath[100];
char outputlog[100];


if(argc!=13)
	{
	cout<<"Please enter valid Arguments\nUsage :\n";
	cout<<"war -t <task> -d < cut_off_depth> -m<map_file> -i <init > -op <output_path> -ol <output_log>"<<endl;
	exit(1);
	}
else
     {
	task=atoi(argv[2]);
	depth=atoi(argv[4]);
	strcpy(mapname,argv[6]);
	strcpy(initmap,argv[8]);
	strcpy(outputpath,argv[10]);
	strcpy(outputlog,argv[12]);

	}




	char source[300],dest[300],cost[300],group[300],node[300];
	string line;
	int edge_count=0,node_count=0;
	int i=0;
	int j=0;
	int k=0,k1=0;
	char *source_arr[100];
	char *dest_arr[100];
	char *nodes[100];
	float cost_arr[100];
	int group_arr[100];




	//char *ranker[100];
	map<string,bool> check;
	ifstream myfile (mapname);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			memset(source,0,300);
			memset(dest,0,300);
			i=0;
			j=0;
			//cout << line << "\n";
			while (line[i] != ',')
			{
				source[j]=line[i];
				i++;
				j++;
			}
			i++;
			j=0;
			while(line[i]>= 'A'&&line[i]<='z')
			{
				dest[j]=line[i];
				i++;
				j++;
			}
			//nncout<<"\n"<<dest<<","<<j<<endl;
			source_arr[k]=new char[300];
			dest_arr[k]=new char[300];
			strcpy(source_arr[k],source);
			strcpy(dest_arr[k],dest); 
			k++;
		}
		myfile.close();
	}
	edge_count=k;
	k=0;
	for(int temp=0;temp<edge_count;temp++)
	{
		//cout<<source_arr[temp]<<"-->"<<dest_arr[temp]<<endl;
	}
	ifstream myfile1(initmap);
	if (myfile1.is_open())
	{
		while ( getline (myfile1,line) )
		{
			memset(node,0,300);
			memset(cost,0,300);	
			memset(group,0,300);
			i=0;
			j=0;

			//cout << line << "\n";
			while (line[i] != ',')
			{
				node[j]=line[i];
				i++;
				j++;
			}
			i++;
			j=0;
			while (line[i] != ',')
			{
				cost[j]=line[i];
				i++;
				j++;
			}
			i++;
			j=0;
			while (line[i] !='\0')
			{
				group[j]=line[i];
				i++;
				j++;
			}
			nodes[k]=new char[300];
			strcpy(nodes[k],node);
			cost_arr[k]=atof(cost); 
			group_arr[k]=atoi(group);
			k++;		
		}
		myfile1.close();
	}

	node_count=k;
	for(int temp1=0;temp1<node_count;temp1++)
	{
		//cout<<nodes[temp1]<<","<<cost_arr[temp1]<<","<<group_arr[temp1]<<endl;
	}

	Vertex *v[100];
	int source_ind=0,dest_ind=0;
	for(int cur=0;cur<node_count;cur++)
	{
		v[cur]=new Vertex(nodes[cur],cost_arr[cur],group_arr[cur]);
		cout<<"Vertex"<<v[cur]->name<<","<<v[cur]->group<<","<<v[cur]->cost<<endl;
	}

	for(int cur=0;cur<node_count;cur++)
	{
		for(int cur1=0;cur1<edge_count;cur1++)
		{	
			if(v[cur]->name==source_arr[cur1])
			{
				for(int i = 0; i < node_count; i++)
				{
					//cout<<"\n1"<<v[i]->name<<","<<dest_arr[cur1]<<endl;
					if(v[i]->name ==dest_arr[cur1])
					{
						//			cout<<"\n2";
						v[cur]->addEdge(v[i]);
						v[i]->addEdge(v[cur]);
					}
				}


			}
		}



	}


opf=fopen(outputpath,"w");
opl=fopen(outputlog,"w");


	Graph g1;
	g1.terminal=depth;
	for(int cur=0;cur<node_count;cur++)
	{
		g1.insert(v[cur]);
	}
	g1.printGraph();
/*	std::list<int> fmpd=g1.fmpdCandidates(-1);

	std::cout << "fmlist contains:";
	for (std::list<int>::iterator it=fmpd.begin(); it!=fmpd.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	std::list<int> pd=g1.pdCandidates(-1);

	std::cout << "pdlist contains:";
	for (std::list<int>::iterator it=pd.begin(); it!=pd.end(); ++it)
		std::cout << ' ' << *it;*/
	std::cout << '\n';

	if(task ==1)
		{
		g1.greedy();
		}
	else if(task ==3)
		{
		g1.Pruned_game();
		}
	else if(task == 2)
		{
		g1.Minimax_game();
		}


}
