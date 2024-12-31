#include "fileNavigation.h"

void fileNavigation::cd(std::string action, fs::path& currentDir){
        action.erase(0,3);
            while(action[0]==' '){
                action.erase(0,1);
            }
            if(action.find("//")==std::string::npos){
                if(action==".."){
                    if(currentDir.filename()=="musicData"){
                        std::cout<<"already at the root level"<<std::endl;
                    }
                    else{
                        currentDir= currentDir.parent_path();
                    }
                }
                else{
                    if(!fs::exists(currentDir / action)){
                        std::cout<<"That is not a valid directory"<<std::endl;
                    }
                    else{
                        currentDir = currentDir / action;
                    }
                }
            }
            else{
                while(action.find("//")!=std::string::npos){
                    for(int i=0; i<action.length(); ++i){
                        if(action[i]=='/'){
                            std::string addOn = action.substr(0,i+1);
                            if(!fs::exists(currentDir / action)){
                                std::cout<<"That is not a valid directory"<<std::endl;
                                action="no";
                                break;
                            }
                            
                            currentDir = currentDir / addOn;
                            action=action.substr(i+1,action.length()-i-1);
                            break;
                        }
                    }
                }
            }

}

void fileNavigation::ls(fs::path& currentDir, std::unordered_map<std::string,std::string>& inventoryRev, std::unordered_map<std::string,std::string>& inventoryRevAlbums){
    bool isEmpty=true;
             for (const auto& entry : fs::directory_iterator(currentDir)) {
                if(entry.is_directory()){
                    std::cout  << "folder: " <<entry.path().filename() << std::endl;
                }
                else{
                    if(inventoryRev.find(entry.path().filename())!=inventoryRev.end()){
                        std::cout<< "playlist: "<<inventoryRev[entry.path().filename()]<<std::endl;
                    }
                    if(inventoryRevAlbums.find(entry.path().filename())!=inventoryRevAlbums.end()){
                        std::cout<<"album: "<<inventoryRevAlbums[entry.path().filename()]<<std::endl;
                    }
                    
                }
                isEmpty=false;
            }
            if(isEmpty){
                std::cout<<"Folder: "<<currentDir.filename()<<" has nothing in it."<<std::endl;
    }
}