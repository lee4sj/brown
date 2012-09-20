
atom_index = 3;
m = p2.weights(atom_index);
simstep = 0.0004;
temperature = 300;

x=squeeze(out.v(atom_index,1,:));
y=squeeze(out.v(atom_index,2,:));
z=squeeze(out.v(atom_index,3,:));


%% problem 1
x2 = x .* x;
y2 = y .* y;
z2 = z .* z;

KE = m * (x2 + y2 + z2);

aprox_boltzmann_1 = mean(KE)/simstep * 2 / 3 / temperature


%% problem 2

atom_index2 = 4;

m = p2.weights(atom_index) / sqrt(2);

x_2=squeeze(out.v(atom_index2,1,:));
y_2=squeeze(out.v(atom_index2,2,:));
z_2=squeeze(out.v(atom_index2,3,:));

x2 = (x-x_2) .* (x-x_2);
y2 = (y-y_2) .* (y-y_2);
z2 = (z-z_2) .* (z-z_2);

KE = m * (x2 + y2 + z2);

aprox_boltzmann_2 = mean(KE)/simstep * 2 / 3 / temperature

hist((x-x_2), 100)


%% problem 3

start_index = 1;
end_index = 20;

m = sum(p2.weights(start_index:end_index));

x=squeeze(out.v(start_index:end_index,1,5001:end));
y=squeeze(out.v(start_index:end_index,2,5001:end));
z=squeeze(out.v(start_index:end_index,3,5001:end));

cmv_x = zeros(1, 20000);
cmv_y = zeros(1, 20000);
cmv_z = zeros(1, 20000);
for k=start_index:end_index
    i = k + 1 - start_index;    
    cmv_x = cmv_x + p2.weights(k) * x(i,:);
    cmv_y = cmv_y + p2.weights(k) * y(i,:); 
    cmv_z = cmv_z + p2.weights(k) * z(i,:);
end

cmv_x = cmv_x / m;
cmv_y = cmv_y / m;
cmv_z = cmv_z / m;

cmv_x2 = cmv_x .* cmv_x;
cmv_y2 = cmv_y .* cmv_y;
cmv_z2 = cmv_z .* cmv_z;

KE = m * (cmv_x2 + cmv_y2 + cmv_z2);

aprox_boltzmann_3 = mean(KE)/simstep * 2 / 3 / temperature


%% problem 4

plot(sum(out.GGB(2200:end, :),2))

energy = out.GGB;
for i=2:size(energy, 1)
    energy(i,1) = (out.GGB(i,1) + out.GGB(i-1,1)) / 2;
end

figure, plot(sum(energy(2200:end, :), 2))








    











