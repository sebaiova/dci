Program ConductorDePrueba;
Var a: integer;
    local: integer;

procedure p(a:boolean);
begin
  p(a);
end;

function pru(r:integer):integer;
{var local: boolean;}
begin
  p(a<>8); 
  if (r= 0) then pru:= 1
  else
     local:= r-1+2; 
 

    a:= pru(a);

end;

Function factorial(r:integer):integer;
begin
   
  a:= pru(a);     {----> ac� da error tb}

  if r = 1 then
    factorial := 1
  else
    {factorial := r * factorial(r-1);}
    factorial := r;
end;

Begin

  a:= pru(a);   {----> ac� da error tb}
  factorial := factorial(1);
  
  a:= local;
  {pru:= 33;}
  local:= pru(33);
  a := factorial(3);
  write(a);
End.
