#ifndef MULTISAMPLER_H
#define MULTISAMPLER_H

#include "ofMain.h"
#include "Sampler.h"

class MultiSampler
{
public:

    MultiSampler(){}

    void setup(){
        samplers.clear();

        string dir_name = "sounds";
        ofDirectory dir(dir_name);
        dir.allowExt("wav");
        dir.listDir();

        for(int i = 0; i < dir.size(); i++){
            string name = dir.getPath(i);

            string d = dir_name+"/";
            size_t pos = name.find(d);
            if (pos != std::string::npos){
                name.erase(pos, d.length());
            }
            for (int i=0; i < 4; i++)
                name.pop_back();

            addSample(name);
        }

    }

    void addSample(string sample_name){
        Sampler sampler;
        sampler.setup(sample_name);
        samplers.insert( std::pair<string, Sampler>(sample_name, sampler) );
    }

    void process(ofSoundBuffer &buffer){

        std::map<string, Sampler>::iterator it = samplers.begin();

        while (it != samplers.end())
        {
            it->second.process(buffer);
            it++;
        }

    }

    void setSample(string sample_name){
        if (samplers.count(sample_name)>0)
            current_sample = sample_name;
    }

    void setParam(string sample_name, string param_name, float value){
        if (samplers.count(sample_name)>0)
            samplers.at(sample_name).setParam(param_name, value);
    }

    void trigger(string sample_name, bool interrupt=true){
        if (samplers.count(sample_name)>0)
            samplers.at(sample_name).trigger(interrupt);
    }

    void draw_info(){

        std::map<string, Sampler>::iterator it = samplers.begin();

        int i = 0;
        float yUnit = (ofGetHeight()-32*2)/samplers.size();
        while (it != samplers.end())
        {
            it->second.draw_info( 32, 32+i*yUnit,ofGetWidth()-32*2, yUnit );
            it++;
            i++;
        }


    }

    string current_sample = "";

private:
    std::map<string, Sampler> samplers;
};

#endif // MULTISAMPLER_H
