p1 = 0.25;
p2 = 0.6;
p3 = 0.9;

x = [1:1:20];

y1 = 1 - p1.^x;
y2 = 1 - p2.^x;
y3 = 1 - p3.^x;

hold on;
plot(x,y1,'LineWidth',1);
plot(x,y2,'LineWidth',1);
plot(x,y3,'LineWidth',1);
legend("p = 0.25 ","p = 0.6 ","p = 0.9 ");
xlabel("Processes Maximum Number");
ylabel("CPU Utilization");
title("CPU Utilization at Different Waiting Time Fraction");
hold off;