class Camera2D{
    private:  
        float x=0.0F,y=0.0F;
        float targetX=0.0F,targetY=0.0F;
        const float defaultZoom = 1.0F,maxZoom=0.90F;
        float zoom = defaultZoom;

        // from 0 to 1 plz
        float cameraSmoothnessX = 0.15F;
        float cameraSmoothnessY = 0.15F;
        float zoomSmoothNess = 0.10F;

    public:
        int W,H;

        Camera2D(int W,int H){this->W = W;this->H = H;}
        Camera2D(float x,float y){setX(0.0F);setY(0.0F);}

        float getX(){return x;}
        float getY(){return y;}

        void setX(float newX){x=newX;}
        void setY(float newY){y=newY;}

        void move(float offsetX,float offsetY){
            setX(x+offsetX);
            setY(y+offsetY);
        }

        void center(int x,int y,int pixelPerUnit){
            setX( (x * pixelPerUnit)- W/2);
            setY( (y * pixelPerUnit) - H/2);
        }

        void target(float x,float y,int pixelPerUnit){
            targetX = ((x*pixelPerUnit) - W/2);
            targetY = ((y*pixelPerUnit)  - H/2);
        }

        /**
         *
         * Smooth Lerp Follow Target camera.
        */
        void slerpFollow(){
            // update position by 15% of the distance between position and target position
            move((targetX- x)*cameraSmoothnessX,(targetY - y)*cameraSmoothnessY);
        }

        void zoomActived(bool active,float deltaTime){ 
            if(active) zoom += (maxZoom-zoom)*zoomSmoothNess*deltaTime;
            else zoom += (defaultZoom-zoom)*zoomSmoothNess*deltaTime;
        }

        float getZoom(){
            return zoom;
        }
};