%y2(i)=PK_0_180_C180(i)*sind(kat2(i));
%x2(i)=PK_0_180_C180(i)*cosd(kat2(i));
M = load("theta_r.txt")
theta = M(:,1);
r = M(:,2);
y = r.*sind(theta);
x = r.*cosd(theta);
%plot(r,theta,'o');
%polarplot(deg2rad(theta),r,'o');
plot(x,y,'o')