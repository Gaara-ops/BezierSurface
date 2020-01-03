$(document).ready(function() {

    //初始化场景
    var scene = new THREE.Scene();
    var camera = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 0.1, 100);
    var cameraControls = new THREE.OrbitControls(camera);
    var renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setClearColor(0xEDEDED);
    renderer.setSize(window.innerWidth, window.innerHeight);
    var axisHelper = new THREE.AxisHelper(20);
    scene.add(axisHelper);

    $(window).on("resize", function() {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
        renderer.setSize(window.innerWidth, window.innerHeight);
        renderScene();
    });

    //相机参数设置
    camera.position.x = -12;
	camera.position.y = 10;
	camera.position.z = -10;
    camera.lookAt(new THREE.Vector3(20,0,15));
    camera.updateProjectionMatrix();
    $(cameraControls).on("change", renderScene);
    cameraControls.target = new THREE.Vector3(18,0,15);

    // UI控制设置
    var controls = new function() {
        this.color = 0x17a6ff;
        this.x = 0.0;
        this.y = 0.0;
        this.z = 0.0;
        this.showControlPoints = false;
        this.isWireframe = false;
        this.bezierCurves = 51;
        this.showAxisHelper = true;
    }


    var gui = new dat.GUI({width:300});

    //曲面颜色
    gui.addColor(controls, "color").name("surface color").onChange(function(c) {
        bezierSurfaceMaterial.color.setHex(c);
        setControlPointsColor(c);
        renderScene();
    });

    //线框渲染
    gui.add(controls, "isWireframe").name("is wireframe").onChange(function(v) {
        bezierSurfaceMaterial.wireframe = v;
        renderScene();
    });

    //坐标轴
    gui.add(controls, "showAxisHelper").name("show axis").onChange(function(v) {
        if(v) scene.add(axisHelper);
        else scene.remove(axisHelper);
        renderScene();
    });

    //控制点
    var controlx, controly, controlz;
    gui.add(controls, "showControlPoints").name("show control points").onChange(function(visibility) {
        if (visibility) {
            controlx = gui.add(controls, "x").listen();
            controly = gui.add(controls, "y").listen();
            controlz = gui.add(controls, "z").listen();
        } else {
            gui.remove(controlx);
            gui.remove(controly);
            gui.remove(controlz);
        }

        updateActivePointControls();
        setControlPointsVisibility(visibility);
        renderScene();
    });

    gui.add(controls, "bezierCurves").min(10).max(100).step(1).name("Division").onFinishChange(function(n) {
            bezierCurveDivisions = n-1;  
            redrawBezierSurface();
            renderScene();
    });


    function updateActivePointControls() {
        controls.x = activeControlPoint.position.x;
        controls.y = activeControlPoint.position.y;
        controls.z = activeControlPoint.position.z;
        controls.x.toFixed(2);
    }





    // 细分
    var bezierCurveDivisions = 50;
    var bezierSurface, bezierSurfaceGeometry, bezierSurfaceMaterial;

    var bezierControlPoints = [
        [new THREE.Vector3(0, 0, 0),
        new THREE.Vector3(10, 0, 0),
        new THREE.Vector3(20, 0, 0),
        new THREE.Vector3(30, 0, 0)],
        [new THREE.Vector3(0, 0, 10),
        new THREE.Vector3(10, 0, 10),
        new THREE.Vector3(20, 0, 10),
        new THREE.Vector3(30, 0, 10)],
        [new THREE.Vector3(0, 0, 20),
        new THREE.Vector3(10,0, 20),
        new THREE.Vector3(20, 0, 20),
        new THREE.Vector3(30, 0, 20)],
        [new THREE.Vector3(0, 0, 30),
        new THREE.Vector3(10, 0, 30),
        new THREE.Vector3(20, 0, 30),
        new THREE.Vector3(30, 0, 30)]
    ]

    function redrawBezierSurface() {
        scene.remove(bezierSurface);

        var basicBezierModel = [];  


        for (var i=0; i < bezierControlPoints.length; i++) {
            var bezier = new THREE.CubicBezierCurve3(
                bezierControlPoints[i][0],
                bezierControlPoints[i][1],
                bezierControlPoints[i][2],
                bezierControlPoints[i][3]
            )
            basicBezierModel.push( bezier.getPoints(bezierCurveDivisions) );
        }


        var bezierCurvesVertices = [];

        
        for (var i=0; i <= bezierCurveDivisions; i++) {
            var bezier = new THREE.CubicBezierCurve3(
                basicBezierModel[0][i],
                basicBezierModel[1][i],
                basicBezierModel[2][i],
                basicBezierModel[3][i]
            )

            bezierCurvesVertices = bezierCurvesVertices.concat( bezier.getPoints(bezierCurveDivisions) );
        }


        
        var bezierSurfaceVertices = bezierCurvesVertices;
        var bezierSurfaceFaces = [];

     
        var v1, v2, v2;  
        for (var i=0; i < bezierCurveDivisions; i++) {
            for (var j=0; j < bezierCurveDivisions; j++) {
                v1 = i * (bezierCurveDivisions + 1) + j;
                v2 = (i+1) * (bezierCurveDivisions + 1) + j;
                v3 = i * (bezierCurveDivisions + 1) + (j+1);
                bezierSurfaceFaces.push( new THREE.Face3(v1, v2, v3) );
                
                v1 = (i+1) * (bezierCurveDivisions + 1) + j;
                v2 = (i+1) * (bezierCurveDivisions + 1) + (j+1);
                v3 = i * (bezierCurveDivisions + 1) + (j+1);
                bezierSurfaceFaces.push( new THREE.Face3(v1, v2, v3) );
            }
        }

        bezierSurfaceGeometry = new THREE.Geometry();
        bezierSurfaceGeometry.vertices = bezierSurfaceVertices;
        bezierSurfaceGeometry.faces = bezierSurfaceFaces;
        bezierSurfaceGeometry.computeFaceNormals();
        bezierSurfaceGeometry.computeVertexNormals();
        bezierSurfaceMaterial = new THREE.MeshLambertMaterial({color: controls.color, wireframe: controls.wireframe});
        bezierSurface = new THREE.Mesh(bezierSurfaceGeometry, bezierSurfaceMaterial);
        bezierSurface.material.side = THREE.DoubleSide;
        scene.add(bezierSurface);
    }

    redrawBezierSurface();





    var activeControlPoint;
    var controlPoints = [];

    (function() {
        for (var i=0; i < 4; i++) {  
            for (var j=0; j < 4; j++) {  
                if (i == 0 && j == 0) {  
                    var controlPointGeometry = new THREE.SphereGeometry(0.4,10,10);
                } else {
                    var controlPointGeometry = new THREE.SphereGeometry(0.4,10,10);
                }
                var controlPointMaterial = new THREE.MeshLambertMaterial({color: 0xffffff - controls.color});
                var controlPoint = new THREE.Mesh(controlPointGeometry, controlPointMaterial);
                controlPoint.name = + i.toString() + "-" + j.toString();
                controlPoint.position.x = bezierControlPoints[i][j].x;
                controlPoint.position.y = bezierControlPoints[i][j].y;
                controlPoint.position.z = bezierControlPoints[i][j].z;
                controlPoint.visible = false;
                if (i==0 && j==0) {
                    controlPoint.scale.set(1.5, 1.5, 1.5);
                    activeControlPoint = controlPoint;
                }

                controlPoints.push(controlPoint);
                scene.add(controlPoint);
            }
        }
    }) ();


    function setControlPointsVisibility(visibility) {
        $.each(controlPoints, function(k, v) {
            v.visible = visibility;
        });
    }


    function setControlPointsColor(color) {
        $.each(controlPoints, function(k, v) {
            v.material.color.setHex(0xffffff - controls.color);
        });
    }


    function updateActiveControlPointPosition(x, y, z) {
        activeControlPoint.position.set(x, y, z);
        var i = activeControlPoint.name.split("-");  
        bezierControlPoints[ i[0] ][ i[1] ] = new THREE.Vector3(x, y, z);
        updateActivePointControls();
        redrawBezierSurface();
        renderScene();
    }


    document.addEventListener("mousedown", onControlPointClick, true);  

    function onControlPointClick(e) {
        var mouse = new THREE.Vector2();
        var raycaster = new THREE.Raycaster();

        mouse.x = (e.clientX / renderer.domElement.width) * 2 - 1;
        mouse.y = - (e.clientY / renderer.domElement.height) * 2 + 1;
        raycaster.setFromCamera(mouse, camera);

        var intersectedObjects = raycaster.intersectObjects(controlPoints);

        if (intersectedObjects.length > 0) {
            cameraControls.enabled = false;  

            var newActiveControlPoint = intersectedObjects[0].object;

            if (newActiveControlPoint.visible) {
                var oldActiveControlPoint = activeControlPoint;

                oldActiveControlPoint.scale.set(1, 1, 1);
                newActiveControlPoint.scale.set(1.5, 1.5, 1.5);

                activeControlPoint = newActiveControlPoint;

                updateActivePointControls();
                renderScene();

                var planeNormal = activeControlPoint.position.clone().sub(camera.position);
                var plane = new THREE.Plane();
                plane.setFromNormalAndCoplanarPoint(planeNormal, activeControlPoint.position);

                $(document).on("mousemove", function(e) {
                    var mouseMove = new THREE.Vector3();
                    mouseMove.x = (e.clientX / renderer.domElement.width) * 2 - 1;
                    mouseMove.y = - (e.clientY / renderer.domElement.height) * 2 + 1;
                    mouseMove.z = 1;

                    mouseMove.unproject(camera);
                    var ray = new THREE.Ray(camera.position, mouseMove.sub(camera.position).normalize());
                    var intersection = ray.intersectPlane(plane);

                    updateActiveControlPointPosition(intersection.x, intersection.y, intersection.z);
                });

                $(document).on("mouseup", function(e) {
                    $(document).off("mousemove");
                    $(document).off("mouseup");
                    cameraControls.enabled = true;  
                });
            }
        }
    }


    var ambientLight = new THREE.AmbientLight(0x0c0c0c);
    scene.add(ambientLight);

    var spotLightBelow = new THREE.SpotLight(0xffffff);
    spotLightBelow.position.set(20, -40, 20);
    spotLightBelow.target = bezierSurface;
    spotLightBelow.exponent = 5;
    scene.add(spotLightBelow);

    var spotLightAbove = new THREE.SpotLight(0xffffff);
    spotLightAbove.position.set(20,40,20);
    spotLightAbove.target = bezierSurface;
    spotLightAbove.exponent = 3;
    scene.add(spotLightAbove);   



    function renderScene() {
        renderer.render(scene, camera);
    }
    
    function update() {
        cameraControls.update();
        requestAnimationFrame(update);
    }
    update();

    $("#WebGL_Canvas").append(renderer.domElement);
    renderScene();
});