f = File.open("d.txt", "r").read
d = File.open("conn.txt", "r").read
f.each_line do |line|
  name = line.split(" ")[0]
  val = line.split(" ")[1]

  #puts "#{name} #{val}"
  d.gsub!(/,  #{name},/,","+val+",")
  d.gsub!(/^#{name},/,val+",")
end
puts d
File.open("dd.txt", "w") {|file| file.puts d}
