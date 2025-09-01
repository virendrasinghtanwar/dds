#include <iostream>
using namespace std;
struct Node{
    int data;
    Node*left;
    Node*right;
    Node*root;
    Node(int val){
        data=val;
        right=nullptr;
        left=nullptr;
    }
};
void inorder(Node*root){
    if(root){
        inorder(root->left);
        cout<<root->data<<" ";
        inorder(root->right);
    }
    
}
void preorder(Node*root){
    if(root){
        cout<<root->data<<" ";
        preorder(root->left);
        preorder(root->right);
    }
}
void postorder(Node*root){
    if(root){
        postorder(root->left);
        postorder(root->right);
        cout<<root->data<<" ";
    }
}

int main() {
    Node * root = new Node(10);
    root-> left=new Node(5);
    root-> right=new Node(9);
    root->left->left=new Node(6);
    root->left->right=new Node(4);
    root->right->left=new Node(3);
    root->right->right= new Node(2);
    
    cout<<"inorder:";
    inorder(root);
    cout<<endl;
    cout<<"preorder:";
    preorder(root); 
    cout<<endl;
    cout<<"postorder ";
    postorder(root);
    

    return 0;
}
