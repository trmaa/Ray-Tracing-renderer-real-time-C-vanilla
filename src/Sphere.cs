using System;

public class Sphere {
	public vec3 position;
	public float radius;
	public vec3 color;

	public Sphere(vec3 position, float radius, vec3 col){
		this.position = position;
		this.radius = radius;
		this.color = col;
	}

	public float colision(Ray ray){
		vec3 oc = ray.origin - this.position;

	    float a = ray.direction.dot(ray.direction);
	    float b = 2 * ray.direction.dot(oc);
	    float c = oc.dot(oc) - this.radius*this.radius;

	    float discriminant = b*b - 4*a*c;

	    if(discriminant<0)  
	    	return 0;
	    else {
		    float[] time = {
		    	(-(float)Math.Sqrt(discriminant)-b)/(2*a),
		    	((float)Math.Sqrt(discriminant)-b)/(2*a)
		    };
		    if(time[0]<=0 || time[1]<=0)
		    	return 0;
		    if(time[0]<time[1])
	            return time[0];
	        else
	            return time[1];
	    }
	}

	public void orbit() {
	    float radius = this.position.modul();
	    float theta = (float)Math.Atan2(this.position.z, this.position.x);
	    float phi = (float)Math.Acos(this.position.y / radius);

	    theta += 2*App.deltaTime;

	    float newX = radius * (float)(Math.Sin(phi) * Math.Cos(theta));
	    float newY = radius * (float)Math.Cos(phi);
	    float newZ = radius * (float)(Math.Sin(phi) * Math.Sin(theta));

	    this.position = new vec3(newX, newY, newZ);
	}
};