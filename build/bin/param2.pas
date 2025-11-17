Program ConductordePrueba;
Var
  c,h,r: integer;
  r2:boolean;

procedure pru2(p: boolean);
begin
  {pru:=18}
  r := 4;
end;

procedure pru3(p: boolean; r22:integer);
begin
  {pru:=18}
  r := 4;
  {r2:= r+h}   {type error ac�}
end;


Begin
  pru3(r2,2+4);
  pru2( ((c > (5*4*r))) or (3<>5) and true or (r>2)  );
  {pru2( ((c > -(5*4*r))) or (3<>5) and true or (r>2)  );}
  if r2 then
  begin
     write(1)
  end
  else
     write(0);
     
End.
