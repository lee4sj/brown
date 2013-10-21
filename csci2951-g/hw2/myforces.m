function F = myforces(pos, phys)

%for each position

forces = zeros(size(pos));

% compute forces of the bonds
bondAtom1 = pos(phys.bonds(:, 1));
bondAtom2 = pos(phys.bonds(:, 2));
K_r = phys.bonds(:, 3);
r_eq = phys.bonds(:, 4);

r = sqrt(sum((bondAtom1 - bondAtom2).^2, 2));
gradBonds = 2 * K_r * (r - r_eq) * (p1 - p2) / r;

% compute forces of the angles
angleAtom1 = pos(phys.angles(:, 1));
angleAtom2 = pos(phys.angles(:, 2));
angleAtom3 = pos(phys.angles(:, 3));

v1 = angleAtom1 - angleAtom2;
v2 = angleAtom3 - angleAtom2;

v1NormO = sqrt(sum(v1.^2, 2));
v2NormO = sqrt(sum(v2.^2, 2));

u = dot(v1, v2, 2) ./ (v1NormO .* v2NormO);
theta = acos(u);

v1Norm = [v1NormO v1NormO v1NormO];
v2Norm = [v2NormO v2NormO v2NormO];

constO = 2 * phys.angles(:, 4) .* (theta - phys.angles(:, 5)) .* (-1 ./ sqrt(1 - u.^2));
const = [constO constO constO];
gradAngleP1 = const ./ (v1Norm .* v2Norm).^2 .* (v1Norm .* v2Norm .* v2 - dot(v1, v2, 2) .* v2Norm ./ v1Norm .* v1);
gradAngleP3 = const ./ (v1Norm .* v2Norm).^2 .* (v1Norm .* v2Norm .* v1 - dot(v1, v2, 2) .* v1Norm ./ v2Norm .* v2);
gradAngleP2 = const ./ (v1Norm .* v2Norm).^2 .* (v1Norm .* v2Norm .* -(v2 + v1) - dot(v1, v2, 2) * (v2Norm ./ v1Norm .* v1 + v1Norm ./ v2Norm .* v2));

% compute dihedral forces
p1 = pos(phys.dihedrals(:, 1));
p2 = pos(phys.dihedrals(:, 2));
p3 = pos(phys.dihedrals(:, 3));
p4 = pos(phys.dihedrals(:, 4));

v1 = p2 - p1;
v2 = p3 - p2;
v3 = p4 - p3;

v2Norm = sqrt(sum(v2.^2, 2));
v1v2Cross = cross(v1, v2, 2);

phi = atan2(v2Norm .* dot(v1, cross(v2, v3, 2), 2), dot(cross(v1, v2, 2), cross(v2, v3, 2), 2);

G1 = v1v2Cross * (v2Norm / myNorm2(v1v2Cross));
G2 = -v1v2Cross * (dot(v1, v2, 2) ./ (v2Norm .* myNorm2(v1v2Cross))) - cross(v2, v3, 2) .* (dot(v2, v3, 2);
    

end

function n = myNorm(v)
n = sqrt(sum(v.^2, 2));
end

function n = myNorm2(v)
n = sum(v.^2, 2);
end

function gradBond = gradientBond(p1, p2, K_r, r_eq)
% grad for bond part is K_r * 2 * (r - r_eq) * ((d/dp1)(r))
% assumes that we are looking for the gradient of p1.

r = sqrt(sum((p1 - p2).^2));
gradBond = K_r * 2 * (r - r_eq) * (1/r) * (p1 - p2);

end

function gradAngle = gradientAngle(p1, p2, p3, K_theta, theta_eq)
% grad for the angle part is K_theta * 2 * (theta - theta_eq) * ((d/dp)(theta))
% ((d/dp) (theta)) = 1/(1-u^2) * ( (p1 * p2) (|p2| / |p1|) (|p1|*|p2|) * p1
% + (|p1|*|p2|)^(-1) * p2

v1 = p1 - p2;
v2 = p3 - p2;

p1Norm = norm(p1);
p2Norm = norm(p2);
p3Norm = norm(p3);

v1Norm = norm(v1);
v2Norm = norm(v2);

u = dot(v1, v2) / (norm(v1) * norm(v2));
theta = acos(u); % by cosine law


const = K_theta * 2 * (theta - theta_eq) * (-1/sqrt(1-u^2));
gradAngleP1 = const / (v1Norm * v2Norm)^2 * (v1Norm * v2Norm * v2 - dot(v1, v2) * v2Norm/v1Norm * v1);
gradAngleP3 = const / (v1Norm * v2Norm)^2 * (v1Norm * v2Norm * v1 - dot(v1, v2) * v1Norm/v2Norm * v2);
gradAngleP2 = const / (v1Norm * v2Norm)^2 * (v1Norm * v2Norm * (v2 + v1) - dot(v1, v2) * (v2Norm/v1Norm * v1 + v1Norm/v2Norm * v2));

gradientAngle = [gradAngleP1; gradAngleP2; gradAngleP3];

end

function phi = gradientDiheralAngle(p1, p2, p3, p4)



end