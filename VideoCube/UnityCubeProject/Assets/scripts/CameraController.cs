using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Vector3 offset;
    public Camera camera;
	public float speed = 0.01f;

    // Start is called before the first frame update
    void Start()
    {
        camera = GetComponent<Camera>();
        offset = transform.position - camera.transform.position;
    }

    // Update is called once per frame
    void LateUpdate()
    {
        transform.position = camera.transform.position + offset;
    }
    
    // Update is called once per frame
    void FixedUpdate()
    {
        float moveHorizontal = Input.GetAxis("Horizontal");
        float moveVertical = Input.GetAxis("Vertical");

        Vector3 movement = new Vector3(moveHorizontal, 0, moveVertical);
		camera.transform.position+=movement * speed;
        //Debug.Log("cameraController: moveHorizontal " + moveHorizontal + " moveVertical" + moveVertical);
        //rb.AddForce(movement * speed);
    }

}
