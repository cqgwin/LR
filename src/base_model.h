// =====================================================================================
// 
//       Filename:  base_model.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09/18/2016 07:53:21 PM
//       Revision:  none
//       Compiler:  g++
// 
// =====================================================================================

#ifndef BASE_MODEL_H_
#define BASE_MODEL_H_

class BaseModel {
 public:
     BaseModel() {}

     virtual ~BaseModel() {}
    
     virtual void SetParameter();

     virtual void Train() = 0;

     virtual void TrainSingleInstance();

     virtual void Predict() = 0;

     virtual void PredictSingleInstance();

     virtual void MultithreadTrain();

     virtual void multithreadPredict();

     virtual void SaveModel() = 0;

     virtual void LoadModel() = 0;
}

#endif
