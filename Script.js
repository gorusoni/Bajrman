const buttons = document.querySelectorAll(".touch");
const greenLight = document.getElementById("green-light");
const redLight = document.getElementById("red-light");
const statusText = document.getElementById("statusText");
const speedBar = document.getElementById("speedBar");
const speedText = document.getElementById("speedText");

let sensors = { c1:false, c2:false, c3:false, c4:false };
let seatbeltOn = false;
let currentSpeed = 0;

buttons.forEach(btn => {
    btn.addEventListener("click", () => {
        const id = btn.textContent.trim();
        btn.classList.toggle("touch_on");

        sensors[id] = !sensors[id];

        checkSeatbelt();
    });
});

function checkSeatbelt(){

    const pair1 = sensors.c1 && sensors.c3 && !sensors.c2 && !sensors.c4;
    const pair2 = sensors.c2 && sensors.c4 && !sensors.c1 && !sensors.c3;

    if(pair1 || pair2){

        seatbeltOn = true;

        greenLight.style.backgroundColor = "lime";
        redLight.style.backgroundColor = "gray";

        statusText.textContent = "✅ Belt secured - Safe driving";

    } else {

        seatbeltOn = false;

        redLight.style.backgroundColor = "red";
        greenLight.style.backgroundColor = "gray";

        statusText.textContent = "❌ Seatbelt not secured (Speed limited)";
    }
}

function move(direction){

    if(direction === "S"){
        setSpeed(0);
        console.log("STOP");
        return;
    }

    if(!seatbeltOn){

        setSpeed(35);
        console.log("Seatbelt OFF → Restricted Speed Mode");

    } else {

        setSpeed(100);
        console.log("Seatbelt ON → Full Speed Mode");

    }

    console.log("Direction:", direction);
}

function setSpeed(percent){

    currentSpeed = percent;

    speedBar.style.width = percent + "%";

    speedText.textContent = "Speed: " + percent + "%";
}